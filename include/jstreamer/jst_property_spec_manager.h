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

class PropertySpecManager {
public:
  const PropertySpec* getPropertySpec(const std::string& property_name) const {
    if (specs_.find(property_name) != specs_.end()) {
      return const_cast<PropertySpec*>(&specs_.at(property_name));
    }
    return nullptr;
  }

  void registerProperty(const PropertySpec& spec) {
    if (!isValid(spec)) {
      throw std::invalid_argument("Invalid property specification");
    }
    specs_[spec.name] = spec;
  }

  bool isRegisteredProperty(const std::string& property_name) const {
    return  specs_.find(property_name) != specs_.end();
  }

  const std::unordered_map<std::string, PropertySpec>& getAllProperties() const {
    return specs_;
  }

  bool isTypeMatch(const std::string& property_name, const std::type_info& type) const {
    if (!isRegisteredProperty(property_name)) {
      return false;
    }
    const auto& spec = specs_.at(property_name);
    return spec.default_value.type() == type && spec.min_value.type() == type && spec.max_value.type() == type;
  }

private:
  static bool isSpecValueTypeMatched(const PropertySpec& spec) {
    return hasSampleType(spec.default_value, spec.min_value) && hasSampleType(spec.min_value, spec.max_value);
  }
  static bool hasSampleType(const std::any& lhs, const std::any& rhs){
    return lhs.type() == rhs.type();
  }

  static bool isValid(const PropertySpec& spec) {
    bool isValid = !spec.name.empty() && !spec.description.empty() &&
           spec.default_value.has_value() && spec.min_value.has_value() && spec.max_value.has_value();
    return isValid && isSpecValueTypeMatched(spec);
  }

  std::unordered_map<std::string, PropertySpec> specs_;
};
}


#endif //JST_PROPERTY_MAP_H
