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
#include <mutex>

class SQLTest : public testing::Test {
 protected:
  static void TearDownTestSuite() {
    cleanup();
    mutex.unlock();
  }

  static void SetUpTestSuite() {
    mutex.lock();
    std::filesystem::remove_all("miniob");
    extern const char *__progname;
    std::string process_name = __progname;
    common::ProcessParam *process_param = common::the_process_param();
    process_param->init_default(process_name);
    process_param->set_conf("../etc/observer_test.ini");
    init(process_param);

    session_stage = (FakeSessionStage *)common::get_seda_config()->get_stage(
        "FakeSessionStage");
  }

  std::string ExecuteSql(const std::string &sql) {
    ConnectionContext ctx;
    memset(&ctx, 0, sizeof(ConnectionContext));
    ctx.session = new Session(Session::default_session());
    pthread_mutex_init(&ctx.mutex, nullptr);
    strncpy(ctx.buf, sql.c_str(), SOCKET_BUFFER_SIZE - 1);
    auto sev = new SessionEvent(&ctx);

    session_stage->add_event(sev);
    session_stage->wait_response();
    return session_stage->get_last_response();
  }

 private:
  static std::mutex mutex;
  static FakeSessionStage *session_stage;
};

std::mutex SQLTest::mutex;
FakeSessionStage *SQLTest::session_stage;

#endif  // SQL_TEST_INIT_H
