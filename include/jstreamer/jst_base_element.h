//
// Created by user on 6/27/25.
//

#ifndef JST_BASE_ELEMENT_H
#define JST_BASE_ELEMENT_H
#include "jstreamer/jst_i_element.h"
#include "jstreamer/jst_property_spec_manager.h"
#include "jstreamer/jst_property_value_manager.h"
#include <cstring>

namespace jstreamer {
class BaseElement : public IElement {
public:
  std::string getName() const override {
    return name_;
  }
  void setName(const std::string &name) override {
    name_ = name;
  }

  void setProperty(const std::string &name, const std::any& value) override {
    property_values_.setProperty(name, value);
  }

  std::optional<std::any> getProperty(const std::string& name) override {
      return property_values_.getProperty(name);
  }

  void registerProperty(const PropertySpec& spec) {
    property_specs_.registerProperty(spec);
    property_values_.loadDefaults();
  }
private:
  std::string name_;
  PropertySpecManager property_specs_;
  PropertyValueManager property_values_{property_specs_};
};
}

#endif //JST_BASE_ELEMENT_H
