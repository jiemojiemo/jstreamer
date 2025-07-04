//
// Created by user on 7/4/25.
//

#ifndef JST_SIGNAL_MANAGER_H
#define JST_SIGNAL_MANAGER_H
#include <eventpp/eventdispatcher.h>
#include <typeindex>

namespace jstreamer {
class SignalManager {
public:
  struct SignalInfo {
    std::string name;
    std::vector<std::type_index> param_types;
  };

  template <typename... Args> void registerSignal(const std::string &name) {
    auto info = SignalInfo{name, {typeid(Args)...}};

    signals_[name] = info;
  }
  template <typename... Args, typename Callback>
  void connect(const std::string &name, Callback &&callback, void* userdata = nullptr) {
    checkArgsMatch(name, {std::type_index(typeid(Args))...});

    auto wrapper = [callback = std::forward<Callback>(callback), userdata, this](const std::vector<std::any>& args) {
      if (sizeof...(Args) != args.size()) {
        throw std::runtime_error("Argument count mismatch");
      }

      if constexpr (std::is_invocable_v<Callback, Args..., void*>) {
        if (userdata != nullptr) {
          call_with_userdata<Args...>(callback, args, userdata, std::index_sequence_for<Args...>{});
        } else {
          call_with_nullptr<Args...>(callback, args, std::index_sequence_for<Args...>{});
        }
      } else if constexpr (std::is_invocable_v<Callback, Args...>) {
        call_without_userdata<Args...>(callback, args, std::index_sequence_for<Args...>{});
      }
    };

    dispatcher_.appendListener(name, wrapper);
  }

  template <typename... Args>
  void emit(const std::string& name, Args... args) {
    checkArgsMatch(name, {std::type_index(typeid(Args))...});
    std::vector<std::any> packed_args = {std::any(args)...};
    dispatcher_.dispatch(name, packed_args);
  }

  size_t getSignalCount() const { return signals_.size(); }

private:
  template<typename... Args, typename Callback, size_t... Is>
  void call_with_userdata(Callback& callback, const std::vector<std::any>& args, void* userdata, std::index_sequence<Is...>) {
    callback(std::any_cast<std::tuple_element_t<Is, std::tuple<Args...>>>(args[Is])..., userdata);
  }

  template<typename... Args, typename Callback, size_t... Is>
  void call_with_nullptr(Callback& callback, const std::vector<std::any>& args, std::index_sequence<Is...>) {
    callback(std::any_cast<std::tuple_element_t<Is, std::tuple<Args...>>>(args[Is])..., nullptr);
  }

  template<typename... Args, typename Callback, size_t... Is>
  void call_without_userdata(Callback& callback, const std::vector<std::any>& args, std::index_sequence<Is...>) {
    callback(std::any_cast<std::tuple_element_t<Is, std::tuple<Args...>>>(args[Is])...);
  }

  void checkArgsMatch(const std::string &name, const std::vector<std::type_index> &args) const {
    auto it = signals_.find(name);
    if (it == signals_.end()) {
      throw std::runtime_error("Signal not registered: " + name);
    }

    if (it->second.param_types != args) {
      throw std::runtime_error("Parameter types mismatch for signal: " + name);
    }
  }
  // 辅助函数：解包参数并调用处理器
  template<typename... Args, typename F, size_t... Is>
   void call_with_unpacked_args(F& func, const std::vector<std::any>& args, void* userdata,
                               std::index_sequence<Is...>) {
    func(std::any_cast<std::tuple_element_t<Is, std::tuple<Args...>>>(args[Is])..., userdata);
  }

  std::unordered_map<std::string, SignalInfo> signals_;
  eventpp::EventDispatcher<std::string, void(const std::vector<std::any>&)> dispatcher_;
};
} // namespace jstreamer

#endif //JST_SIGNAL_MANAGER_H
