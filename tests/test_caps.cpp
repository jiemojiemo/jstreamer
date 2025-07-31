//
// Created by user on 7/29/25.
//
#include <gmock/gmock.h>

#include "jstreamer/jst_caps.h"


using namespace testing;
using namespace jstreamer;

class ACaps : public Test {
public:
};

TEST_F(ACaps, StructureIsEmptyWhenInit) {
  Caps caps;
  ASSERT_THAT(caps.getSize(), Eq(0));
}

TEST_F(ACaps, ConstroctWithMimetypeHasSizeOneStructure) {
  Caps caps("video/x-raw");

  ASSERT_THAT(caps.getSize(), Eq(1));
}

TEST_F(ACaps, CanGetStructureByIndex) {
  Caps caps("video/x-raw");

  auto* s = caps.getStructure(0);
  ASSERT_THAT(s->getName(), StrEq("video/x-raw"));
}

TEST_F(ACaps, GetStructureReturnsNullIfIndexOutOfRange) {
  Caps caps("video/x-raw");

  auto* s = caps.getStructure(1);
  ASSERT_THAT(s, IsNull());
}

TEST_F(ACaps, ConstructorWithMimeTypeIsNotAny) {
  Caps caps("video/x-raw");

  ASSERT_FALSE(caps.isAny());
}

TEST_F(ACaps, CanCreateAnyCaps) {
  auto caps = Caps::any();

  ASSERT_TRUE(caps.isAny());
}

TEST_F(ACaps, AppendStructureIncreaseSize) {
  Caps caps;
  ASSERT_THAT(caps.getSize(), Eq(0));

  auto s = Structure("video/x-raw");
  caps.appendStructure(s);

  ASSERT_THAT(caps.getSize(), Eq(1));
}