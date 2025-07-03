//
// Created by user on 6/27/25.
//
#include <gmock/gmock.h>
#include "jstreamer/jst_property_map.h"

using namespace testing;

class APropertyMap : public Test {
public:
  jstreamer::PropertyMap properties;
};

TEST_F(APropertyMap, getPropertySpecReturnsNullIfNeverRegresitered) {
  auto meta = properties.getPropertySpec("non_existent_property");
  ASSERT_THAT(meta, IsNull());
}

TEST_F(APropertyMap, canGetPropertySpecAfterRegistration) {
  jstreamer::PropertySpec spec = {"test_property", "Test Property", 42, 0, 100};
  properties.registerProperty(spec);

  auto meta = properties.getPropertySpec(spec.name);

  ASSERT_THAT(meta, NotNull());
  ASSERT_THAT(meta->name, Eq(spec.name));
  ASSERT_THAT(meta->description, Eq(spec.description));
  ASSERT_THAT(std::any_cast<int>(meta->default_value), Eq(42));
}

TEST_F(APropertyMap, getPropertyValueWithDefaultValueAfterRegistration) {
  jstreamer::PropertySpec spec = {"property", "Another Property", 3.14f, 0.0f, 10.0f};
  properties.registerProperty(spec);

  auto value = properties.getPropertyValue(spec.name);

  ASSERT_TRUE(value.has_value());
  ASSERT_TRUE(value->type() == typeid(float));
  ASSERT_THAT(std::any_cast<float>(*value), FloatEq(3.14f));
}

TEST_F(APropertyMap, getNullPropertyIfNameNotRegistered) {
  jstreamer::PropertySpec spec = {"property", "Another Property", 3.14f, 0.0f, 10.0f};
  properties.registerProperty(spec);

  auto value = properties.getPropertyValue("non_existent_property");

  ASSERT_FALSE(value.has_value());
}