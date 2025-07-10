//
// Created by user on 7/9/25.
//
#include <gmock/gmock.h>
#include <jstreamer/jst_structure.h>
#include <jstreamer/jst_type.h>

using namespace jstreamer;
using namespace testing;

class AStructure : public Test {
public:
  Structure structure = Structure("test");
};

TEST_F(AStructure, CanGetName) {
  ASSERT_THAT(structure.getName(), Eq("test"));
}

TEST_F(AStructure, FieldIsEmptyWhenInit) {
  ASSERT_THAT(structure.getFieldNames().size(), Eq(0));
}

// 可重构点：减少重复断言代码，提升可读性
// 1. 针对 SetFiledSuccessIfTypeSupported 测试，使用结构体和循环批量断言
// 2. 针对 SetFieldIncreaseFieldCount、RemoveFieldDecreasesFieldCount、CanRemoveAllFields 等，使用辅助函数减少重复
TEST_F(AStructure, SetFiledSuccessIfTypeSupported) {
  struct FieldTest {
    std::string name;
    std::any value;
  };
  std::vector<FieldTest> fields = {
    {"jst_bool", static_cast<jst_bool>(true)},
    {"jst_int", static_cast<jst_int>(0)},
    {"jst_int64", static_cast<jst_int64>(0)},
    {"jst_string", jst_string("abc")},
    {"jst_float", static_cast<jst_float>(0)},
    {"jst_double", static_cast<jst_double>(0)},
    {"jst_range", jst_range(0, 100)}
  };
  for (const auto& f : fields) {
    structure.setField(f.name, f.value);
    ASSERT_TRUE(structure.getField(f.name).has_value()) << f.name << " field should be set successfully";
  }
}

TEST_F(AStructure, SetFiledThrowsIfTypeNotSupported) {
  auto values_not_support = std::vector<std::any>();

  ASSERT_THROW(structure.setField("a", values_not_support), std::runtime_error);
}

TEST_F(AStructure, SetFieldIncreaseFieldCount) {
  ASSERT_THAT(structure.getFieldNames().size(), Eq(0));

  structure.setField("a", 1);
  structure.setField("b", 2);

  ASSERT_THAT(structure.getFieldNames().size(), Eq(2));
}

TEST_F(AStructure, CanCheckAFieldExists) {
  structure.setField("test_field", 42);

  ASSERT_TRUE(structure.hasField("test_field"));
  ASSERT_FALSE(structure.hasField("non_existent_field"));
}



TEST_F(AStructure, GetFieldReturnNulloptIfNameNotFound) {
  auto field_name = "abc";
  ASSERT_FALSE(structure.hasField(field_name));

  auto field = structure.getField(field_name);
  ASSERT_FALSE(field.has_value());
}


TEST_F(AStructure, GetFieldWhenNameFound) {
  auto field_name = "test_field";
  structure.setField(field_name, 42);

  auto field = structure.getField(field_name);
  ASSERT_TRUE(field.has_value());
  ASSERT_TRUE(field->type() == typeid(int));
  ASSERT_THAT(std::any_cast<int>(*field), Eq(42));
}

TEST_F(AStructure, RemoveFieldDecreasesFieldCount) {
  structure.setField("test_field", 42);
  structure.setField("test_field2", 100);
  ASSERT_THAT(structure.getFieldNames().size(), Eq(2));

  structure.removeField("test_field");
  ASSERT_THAT(structure.getFieldNames().size(), Eq(1));
  structure.removeField("test_field2");
  ASSERT_THAT(structure.getFieldNames().size(), Eq(0));
}

TEST_F(AStructure, RemoveFieldDoesNothingIfNameNotFound) {
  structure.setField("test_field", 42);
  ASSERT_THAT(structure.getFieldNames().size(), Eq(1));

  structure.removeField("non_existent_field");
  ASSERT_THAT(structure.getFieldNames().size(), Eq(1));
}

TEST_F(AStructure, CanRemoveAllFields) {
  structure.setField("test_field", 42);
  structure.setField("test_field2", 100);
  ASSERT_THAT(structure.getFieldNames().size(), Eq(2));

  structure.removeAllFields();

  ASSERT_THAT(structure.getFieldNames().size(), Eq(0));
}

TEST_F(AStructure, IsNotEqualIfNameIsDifferent) {
  Structure s1("test1");
  Structure s2("test2");

  ASSERT_FALSE(s1.isEqual(s2));
}

TEST_F(AStructure, IsNotEqualIfFieldCountIsDifferent) {
  Structure s1("test");
  Structure s2("test");

  s1.setField("field1", 42);

  ASSERT_FALSE(s1.isEqual(s2));
}

TEST_F(AStructure, IsNotEqualIfFiledNamesAreDifferent) {
  Structure s1("test");
  Structure s2("test");

  s1.setField("field1", 42);
  s2.setField("field2", 42);

  ASSERT_FALSE(s1.isEqual(s2));
}

TEST_F(AStructure, IsNotEqualIfFiledValuesAreDifferent) {
  Structure s1("test");
  Structure s2("test");

  s1.setField("field1", 42);
  s2.setField("field1", 100);

  ASSERT_FALSE(s1.isEqual(s2));
}

TEST_F(AStructure, IsNotEuqalIfFieldTypesAreDifferent) {
  Structure s1("test");
  Structure s2("test");

  s1.setField("field1", 42); // int
  s2.setField("field1", 42.0f); // float

  ASSERT_FALSE(s1.isEqual(s2));
}
