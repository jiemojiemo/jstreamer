//
// Created by user on 7/4/25.
//
#include <gmock/gmock.h>
#include "jstreamer/jst_signal_manager.h"

using namespace jstreamer;
using namespace testing;

class ASignalManager : public Test {
public:
  SignalManager s = SignalManager();
};

TEST_F(ASignalManager, EmptySignalWhenInit) {
  ASSERT_THAT(s.getSignalCount(), Eq(0));
}

TEST_F(ASignalManager, RegisterIncreaseSignalCount) {
  auto name = "signal";
  s.registerSignal<int>(name);

  ASSERT_THAT(s.getSignalCount(), Eq(1));
}

TEST_F(ASignalManager, ConnectThrowsIfNameNotRegistered) {
  auto name = "signal";
  auto callback = [](int signal) {};

  ASSERT_THROW(s.connect<int>(name, callback), std::runtime_error);
}

TEST_F(ASignalManager, ConnectThrowsIfArgsNotMatchWithRigsteredSignal) {
  auto name = "signal";
  s.registerSignal<int>(name);

  auto callback = [](double signal) {};

  ASSERT_THROW(s.connect<double>(name, callback), std::runtime_error);
}

TEST_F(ASignalManager, EmitThrowsIfNameNotRegistered) {
  auto name = "signal";

  ASSERT_THROW(s.emit<int>(name, 1), std::runtime_error);
}

TEST_F(ASignalManager, EmitThrowsIfArgsNotMatchWithRegisteredSignal) {
  auto name = "signal";
  s.registerSignal<int>(name);

  ASSERT_THROW(s.emit<double>(name, 1.0), std::runtime_error);
}

TEST_F(ASignalManager, EmitTriggerConnectedCallbackIfRegistered) {
  auto name = "signal0";
  s.registerSignal<int>(name);

  auto got = 0;
  s.connect<int>(name, [&](int signal) {
    got = signal;
  });
  s.emit<int>(name, 1);

  ASSERT_THAT(got, 1);
}

TEST_F(ASignalManager, CanConnectWithUserData) {
  auto name = "signal";
  auto userdata = 42;
  s.registerSignal<int>(name);

  s.connect<int>(name, [&](int signal) {}, &userdata);
}

TEST_F(ASignalManager, EmitTriggerConnectedCallbackWithUserData) {
  auto name = "signal";
  auto userdata = 42;
  s.registerSignal<int>(name);

  int got_userdata = 0;
  s.connect<int>(name, [&](int signal, void* user_data) {
    got_userdata = *static_cast<int*>(user_data);
  }, &userdata);

  s.emit<int>(name, 1);

  ASSERT_THAT(got_userdata, Eq(userdata));
}

TEST_F(ASignalManager, CanGetSignalInfo) {
  auto name = "signal";
  s.registerSignal<int>(name);

  auto signals = s.getSignals();

  ASSERT_THAT(signals.size(), Eq(1));
  ASSERT_THAT(signals["signal"].name, "signal");
  ASSERT_THAT(signals["signal"].param_types.size(), 1);
}