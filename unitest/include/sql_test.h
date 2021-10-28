//
// Created by huaouo on 10/23/21.
//

#ifndef SQL_TEST_INIT_H
#define SQL_TEST_INIT_H

#include <common/metrics/metrics_registry.h>
#include <common/seda/seda_config.h>
#include <common/seda/stage.h>
#include <common/seda/stage_factory.h>
#include <event/session_event.h>
#include <event/sql_event.h>
#include <gtest/gtest.h>
#include <ini_setting.h>
#include <init.h>
#include <session/fake_session_stage.h>
#include <session/session.h>
#include <session/session_stage.h>

#include <filesystem>
#include <memory>
#include <mutex>

class SQLTest : public testing::Test {
 protected:
  static void TearDownTestSuite() {
    cleanup();
    std::filesystem::remove_all(db_path);
  }

  static void SetUpTestSuite() {
    std::filesystem::remove_all(db_path);
    extern const char *__progname;
    std::string process_name = __progname;
    common::ProcessParam *process_param = common::the_process_param();
    process_param->init_default(process_name);
    process_param->set_conf("../etc/observer_test.ini");
    db_path = std::string("./miniob_") + __progname;
    common::get_properties()->put("BaseDir", db_path, "DefaultStorageStage");
    init(process_param);

    session_stage = (FakeSessionStage *)common::get_seda_config()->get_stage(
        "FakeSessionStage");
  }

  void TearDown() final {
    AfterCase();
    mutex.unlock();
  }

  void SetUp() final {
    mutex.lock();
    BeforeCase();
  }

  virtual void BeforeCase() {}

  virtual void AfterCase() {}

  static std::string ExecuteSql(const std::string &sql) {
    ConnectionContext ctx;
    memset(&ctx, 0, sizeof(ConnectionContext));
    auto session = std::make_shared<Session>(Session::default_session());
    ctx.session = session.get();
    pthread_mutex_init(&ctx.mutex, nullptr);
    strncpy(ctx.buf, sql.c_str(), SOCKET_BUFFER_SIZE - 1);
    auto sev = new SessionEvent(&ctx);

    session_stage->add_event(sev);
    return session_stage->wait_response();
  }

 private:
  std::mutex mutex;
  static std::string db_path;
  static FakeSessionStage *session_stage;
};

std::string SQLTest::db_path;
FakeSessionStage *SQLTest::session_stage;

#endif  // SQL_TEST_INIT_H
