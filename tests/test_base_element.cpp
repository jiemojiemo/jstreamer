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
};

TEST_F(ABaseElement, HasEmptyNameWhenInit) {
  ASSERT_THAT(e.getName(), IsEmpty());
}

TEST_F(ABaseElement, CanSetNewName) {
  auto new_name = "abc";
  e.setName(new_name);

  ASSERT_THAT(e.getName(), Eq(new_name));
}