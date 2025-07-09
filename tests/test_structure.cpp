//
// Created by user on 7/9/25.
//
#include <gmock/gmock.h>
#include <jstreamer/jst_structure.h>

using namespace jstreamer;
using namespace testing;

class AStructure : public Test {
public:
  Structure structure = Structure();
};

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