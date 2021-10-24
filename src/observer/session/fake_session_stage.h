//
// Created by huaouo on 10/24/21.
//

#ifndef MINIDB_FAKE_SESSION_STAGE_H
#define MINIDB_FAKE_SESSION_STAGE_H

#include <common/metrics/metrics_registry.h>
#include <common/seda/callback.h>
#include <common/seda/seda_config.h>
#include <common/seda/stage.h>
#include <common/seda/stage_factory.h>
#include <event/session_event.h>
#include <event/sql_event.h>
#include <ini_setting.h>
#include <init.h>
#include <session/fake_session_stage.h>
#include <session/session_stage.h>

#include <condition_variable>
#include <mutex>

// For test purpose only
class FakeSessionStage : public common::Stage {
 protected:
  void callback_event(common::StageEvent *event,
                      common::CallbackContext *context) override {
    auto sev = dynamic_cast<SessionEvent *>(event);
    if (nullptr == sev) {
      LOG_ERROR("Cannot cat event to sessionEvent");
      return;
    }

    std::lock_guard<std::mutex> lock(mutex);
    const char *response = sev->get_response();
    int len = sev->get_response_len();
    if (len <= 0 || response == nullptr) {
      response = "No data\n";
    }
    last_response = response;
    sev->done();
    cv.notify_one();
  }

  bool initialize() override {
    std::list<Stage *>::iterator stgp = next_stage_list_.begin()++;
    resolve_stage_ = *(stgp++);

    common::MetricsRegistry &metricsRegistry = common::get_metrics_registry();
    sql_metric_ = new common::SimpleTimer();
    metricsRegistry.register_metric(SQL_METRIC_TAG, sql_metric_);
    return true;
  }

  void handle_event(common::StageEvent *event) override {
    auto sev = dynamic_cast<SessionEvent *>(event);
    if (nullptr == sev) {
      LOG_ERROR("Cannot cat event to sessionEvent");
      return;
    }

    common::TimerStat sql_stat(*sql_metric_);
    if (nullptr == sev->get_request_buf()) {
      LOG_ERROR("Invalid request buffer.");
      sev->done_immediate();
      return;
    }

    std::string sql = sev->get_request_buf();
    if (common::is_blank(sql.c_str())) {
      sev->done_immediate();
      return;
    }

    auto cb = new (std::nothrow) common::CompletionCallback(this, nullptr);
    if (cb == nullptr) {
      LOG_ERROR("Failed to new callback for SessionEvent");

      sev->done_immediate();
      return;
    }

    sev->push_callback(cb);

    auto sql_event = new SQLStageEvent(sev, sql);
    resolve_stage_->handle_event(sql_event);
  }

 public:
  std::string get_last_response() { return last_response; }

  static Stage *make_stage(const std::string &tag) {
    auto stage = new (std::nothrow) FakeSessionStage(tag.c_str());
    if (stage == nullptr) {
      LOG_ERROR("new ExecutorStage failed");
      return nullptr;
    }
    stage->set_properties();
    return stage;
  }

  void wait_response() {
    std::unique_lock<std::mutex> lock(mutex);
    cv.wait(lock);
  }

 private:
  explicit FakeSessionStage(const char *tag)
      : Stage(tag), resolve_stage_(nullptr), sql_metric_(nullptr) {}

  const std::string SQL_METRIC_TAG = "FakeSessionStage.sql";
  std::string last_response;
  common::Stage *resolve_stage_;
  common::SimpleTimer *sql_metric_;

  std::mutex mutex;
  std::condition_variable cv;
};

#endif  // MINIDB_FAKE_SESSION_STAGE_H
