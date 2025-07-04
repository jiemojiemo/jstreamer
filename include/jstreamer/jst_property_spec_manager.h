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
    specs_[spec.name] = spec;
  }

  bool isRegisteredProperty(const std::string& property_name) const {
    return  specs_.find(property_name) != specs_.end();
  }

  const std::unordered_map<std::string, PropertySpec>& getAllProperties() const {
    return specs_;
  }

private:
  std::unordered_map<std::string, PropertySpec> specs_;
};
}


#endif //JST_PROPERTY_MAP_H
