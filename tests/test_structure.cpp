//
// Created by user on 7/9/25.
//
#include <gmock/gmock.h>
#include <jstreamer/jst_structure.h>

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

TEST_F(AStructure, SetFieldIncreaseFieldCount) {
  struct Field {
    std::string name;
    std::any value;
  };
  std::vector<Field> fields = {
    {"test_field_int", 42},
    {"test_field_float", 42.0f},
    {"test_field_double", 42.0},
    {"test_field_string", std::string("42")}
  };
  for (const auto& f : fields) {
    structure.setField(f.name, f.value);
  }

  ASSERT_THAT(structure.getFieldNames().size(), Eq(fields.size()));
  for (const auto& f : fields) {
    ASSERT_THAT(structure.getFieldNames(), Contains(f.name));
  }
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