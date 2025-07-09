//
// Created by user on 6/27/25.
//
#include <gmock/gmock.h>
#include "jstreamer/jst_base_element.h"

using namespace testing;
using namespace jstreamer;

class ABaseElement : public Test {
public:
  BaseElement e;
  PropertySpec spec = PropertySpec{"a", "desc", 10, 0, 100};
};

TEST_F(ABaseElement, HasEmptyNameWhenInit) {
  ASSERT_THAT(e.getName(), IsEmpty());
}

TEST_F(ABaseElement, CanSetNewName) {
  auto new_name = "abc";
  e.setName(new_name);

  ASSERT_THAT(e.getName(), Eq(new_name));
}

TEST_F(ABaseElement, SetPropertyThrowsIfNameNotFound) {
  auto name = "nonexistent";
  auto value = "value";

  ASSERT_THROW(e.setProperty(name, value), std::runtime_error);
}

TEST_F(ABaseElement, SetPropertyValueIfNameRegisterd) {
  e.registerProperty(spec);

  e.setProperty(spec.name, 20);

  auto got = e.getProperty(spec.name);

  ASSERT_TRUE(got.has_value());
  ASSERT_THAT(std::any_cast<int>(*got), Eq(20));
}

TEST_F(ABaseElement, registerPropertyAlsoRegiterASignal) {
  auto signals = e.getSignals();
  ASSERT_THAT(signals.size(), Eq(0));

  e.registerProperty(spec);

  signals = e.getSignals();
  ASSERT_THAT(signals.size(), Eq(1));
}

TEST_F(ABaseElement, registerPropertyAlsoRegiterASignalWithNotifyName) {
  auto signals = e.getSignals();
  ASSERT_THAT(signals.size(), Eq(0));

  e.registerProperty(spec);

  signals = e.getSignals();
  auto expected_signal_name = "notify::" + spec.name;
  ASSERT_THAT(signals.count(expected_signal_name), Eq(1));
}

TEST_F(ABaseElement, SetPropertyNotifySignal) {
  e.registerProperty(spec);

  auto signal_name = "notify::" + spec.name;
  auto got_signal = false;
  auto callback = [&](IElement*, const PropertySpec*) {
    got_signal = true;
  };
  e.signalConnect<IElement*, const PropertySpec*>(signal_name, callback);

  e.setProperty(spec.name, 20);

  ASSERT_THAT(got_signal, Eq(true));
}

TEST_F(ABaseElement, SetPropertyNotifySignalWithUserData) {
  e.registerProperty(spec);

  auto signal_name = "notify::" + spec.name;
  auto got_signal = false;
  auto userdata = 42;
  auto callback = [&](IElement*, const PropertySpec*, void* user_data) {
    got_signal = true;
    ASSERT_THAT(*static_cast<int*>(user_data), Eq(userdata));
  };
  e.signalConnect<IElement*, const PropertySpec*>(signal_name, callback, &userdata);

  e.setProperty(spec.name, 20);

  ASSERT_THAT(got_signal, Eq(true));
}