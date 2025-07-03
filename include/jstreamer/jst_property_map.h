//
// Created by user on 7/3/25.
//

#ifndef JST_PROPERTY_MAP_H
#define JST_PROPERTY_MAP_H
#include <string>
#include <optional>
#include <any>
namespace jstreamer {

struct PropertySpec {
  std::string name;
  std::string description;
  std::any default_value;
  std::any min_value;
  std::any max_value;
};

class PropertyMap {
public:
  const PropertySpec* getPropertySpec(const std::string& property_name) const {
    if (specs_.find(property_name) != specs_.end()) {
      return const_cast<PropertySpec*>(&specs_.at(property_name));
    }
    return nullptr;
  }

  void registerProperty(const PropertySpec& spec) {
    specs_[spec.name] = spec;
    values_[spec.name] = spec.default_value;
  }

  std::optional<std::any> getPropertyValue(const std::string& name) const {
    if (values_.find(name) != values_.end()) {
      return values_.at(name);
    }else {
      return std::nullopt;
    }
  }

private:
  std::unordered_map<std::string, PropertySpec> specs_;
  std::unordered_map<std::string, std::any> values_;
};
}


#endif //JST_PROPERTY_MAP_H
