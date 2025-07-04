//
// Created by user on 7/3/25.
//
#include <gmock/gmock.h>
#include <jstreamer/jst_property_spec_manager.h>
#include <jstreamer/jst_property_value_manager.h>

using namespace jstreamer;
using namespace testing;

class APropertyValueManager : public Test {
public:
  PropertySpecManager specManager = PropertySpecManager();
};

TEST_F(APropertyValueManager, CanInitWithSpecManager) {
  auto valueManager = PropertyValueManager(specManager);
}

TEST_F(APropertyValueManager, GetPropertyValueFailedIfSpecNotRegistered) {
  auto valueManager = PropertyValueManager(specManager);

  auto value = valueManager.getProperty("non_existent_property");
  ASSERT_FALSE(value.has_value());
}

TEST_F(APropertyValueManager, GetPropertyValueIfSpecRegistered) {
  PropertySpec spec = {"test_property", "Test Property", 42, 0, 100};
  specManager.registerProperty(spec);

  auto valueManager = PropertyValueManager(specManager);
  auto value = valueManager.getProperty("test_property");

  ASSERT_TRUE(value.has_value());
  ASSERT_TRUE(value->type() == typeid(int));
  ASSERT_THAT(std::any_cast<int>(*value), Eq(42));
}

TEST_F(APropertyValueManager, SetPropertyFailedIfSpecNotRegistered) {
  PropertyValueManager valueManager(specManager);

  // Attempt to set a property that is not registered
  auto value = valueManager.getProperty("non_existent_property");
  ASSERT_FALSE(value.has_value());
}