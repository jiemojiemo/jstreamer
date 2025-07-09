//
// Created by user on 7/4/25.
//

#ifndef JST_PROPERTY_VALUE_MANAGER_H
#define JST_PROPERTY_VALUE_MANAGER_H
#include <jstreamer/jst_property_spec_manager.h>
namespace jstreamer {
class PropertyValueManager {
public:
  explicit PropertyValueManager(const PropertySpecManager& specManager)
      : specManager_(specManager)
  {
    loadDefaults();
  }

  std::optional<std::any> getProperty(const std::string& name) const {
    if (!specManager_.isRegisteredProperty(name)) {
      return std::nullopt;
    }
    return values_.at(name);
  }

  void setProperty(const std::string& name, const std::any& value) {
    if (!specManager_.isRegisteredProperty(name)) {
      throw std::runtime_error("Property not registered: " + name);
    }

    if(!specManager_.isTypeMatch(name, value.type())) {
      throw std::runtime_error("Type mismatch for property: " + name);
    }

    values_[name] = value;
  }

  const std::unordered_map<std::string, std::any>& getAllValues() const {
    return values_;
  }

  std::unordered_map<std::string, std::any> getAllValues() {
    return values_;
  }

  void loadDefaults() {
    for (const auto& [name, spec] : specManager_.getAllProperties()) {
      // Initialize values with default values from specs
      values_[name] = spec.default_value;
    }
  }

private:
  const PropertySpecManager& specManager_;
  std::unordered_map<std::string, std::any> values_;
};
}


#endif //JST_PROPERTY_VALUE_MANAGER_H
