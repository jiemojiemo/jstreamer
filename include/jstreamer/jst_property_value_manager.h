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
    for (const auto& [name, spec] : specManager_.getAllProperties()) {
      // Initialize values with default values from specs
      values_[name] = spec.default_value;
    }
  }

  std::optional<std::any> getProperty(const std::string& name) const {
    if (!specManager_.isRegisteredProperty(name)) {
      return std::nullopt;
    }
    return values_.at(name);
  }

private:
  const PropertySpecManager& specManager_;
  std::unordered_map<std::string, std::any> values_;
};
}


#endif //JST_PROPERTY_VALUE_MANAGER_H
