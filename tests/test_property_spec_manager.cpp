//
// Created by user on 6/27/25.
//
#include <gmock/gmock.h>
#include "jstreamer/jst_property_spec_manager.h"

using namespace testing;

class APropertyManager : public Test {
public:
  jstreamer::PropertySpecManager properties;
};

TEST_F(APropertyManager, getPropertySpecReturnsNullIfNeverRegresitered) {
  auto meta = properties.getPropertySpec("non_existent_property");
  ASSERT_THAT(meta, IsNull());
}

TEST_F(APropertyManager, canGetPropertySpecAfterRegistration) {
  jstreamer::PropertySpec spec = {"test_property", "Test Property", 42, 0, 100};
  properties.registerProperty(spec);

  auto meta = properties.getPropertySpec(spec.name);

  ASSERT_THAT(meta, NotNull());
  ASSERT_THAT(meta->name, Eq(spec.name));
  ASSERT_THAT(meta->description, Eq(spec.description));
  ASSERT_THAT(std::any_cast<int>(meta->default_value), Eq(42));
}

TEST_F(APropertyManager, canCheckIfPropertyRegistered) {
  jstreamer::PropertySpec spec = {"test_property", "Test Property", 42, 0, 100};
  properties.registerProperty(spec);

  ASSERT_TRUE(properties.isRegisteredProperty("test_property"));
  ASSERT_FALSE(properties.isRegisteredProperty("abc"));
}

TEST_F(APropertyManager, registerThrowsIfValuesTypeNotMatch) {
  auto defaultValue = 1.0;
  auto minValue = 0;
  auto maxValue = 100L;
  jstreamer::PropertySpec spec = {"test_property", "Test Property", defaultValue, minValue, maxValue};

  ASSERT_THROW(properties.registerProperty(spec), std::invalid_argument);
}

TEST_F(APropertyManager, registerThrowsIfPropertyNameIsEmpty) {
  jstreamer::PropertySpec spec = {"", "Test Property", 42, 0, 100};

  ASSERT_THROW(properties.registerProperty(spec), std::invalid_argument);
}