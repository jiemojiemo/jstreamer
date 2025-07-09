//
// Created by user on 6/27/25.
//
#include <gmock/gmock.h>
#include "jstreamer/jst_base_element.h"

using namespace testing;
using namespace jstreamer;

class MockBaseElement : public BaseElement {
public:
  MockBaseElement()
  {

  }
};

class ABaseElement : public Test {
public:
  BaseElement e;
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
  auto spec = PropertySpec{"a", "desc", 10, 0, 100};
  e.registerProperty(spec);

  e.setProperty(spec.name, 20);

  auto got = e.getProperty(spec.name);

  ASSERT_TRUE(got.has_value());
  ASSERT_THAT(std::any_cast<int>(*got), Eq(20));
}