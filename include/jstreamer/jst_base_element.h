//
// Created by user on 6/27/25.
//

#ifndef JST_BASE_ELEMENT_H
#define JST_BASE_ELEMENT_H
#include "jstreamer/jst_i_element.h"
#include "jstreamer/jst_property_spec_manager.h"
#include "jstreamer/jst_property_value_manager.h"
#include "jstreamer/jst_signal_manager.h"
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

    emitPropertySignal(name);
  }

  std::optional<std::any> getProperty(const std::string& name) override {
      return property_values_.getProperty(name);
  }

  void registerProperty(const PropertySpec& spec) {
    property_specs_.registerProperty(spec);
    property_values_.loadDefaults();
    registerPropertySignal(spec);
  }

  const std::unordered_map<std::string, SignalManager::SignalInfo>& getSignals() const {
    return signals_manager_.getSignals();
  }

  template <typename... Args, typename Callback>
  void signalConnect(const std::string &name, Callback &&callback, void* userdata = nullptr) {
    signals_manager_.connect<Args...>(name, callback, userdata);
  }

private:
  void registerPropertySignal(const PropertySpec& spec) {
    auto signalName = buildPropertyNotifyName(spec.name);
    signals_manager_.registerSignal<IElement*, const PropertySpec*>(signalName);
  }
  void emitPropertySignal(const std::string &name) {
    auto signal_name = buildPropertyNotifyName(name);
    signals_manager_.emit(signal_name, reinterpret_cast<IElement*>(this), property_specs_.getPropertySpec(name));
  }
  static std::string buildPropertyNotifyName(const std::string &name) {
    return "notify::" + name;
  }
  std::string name_;
  PropertySpecManager property_specs_;
  PropertyValueManager property_values_{property_specs_};
  SignalManager signals_manager_;
};
}

#endif //JST_BASE_ELEMENT_H
