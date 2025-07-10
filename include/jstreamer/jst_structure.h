//
// Created by user on 7/9/25.
//

#ifndef JST_STRUCTURE_H
#define JST_STRUCTURE_H

#include "jst_type.h"

#include <any>
#include <optional>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace jstreamer {
class Structure {
public:
    explicit Structure(std::string name) : name_(std::move(name)) {}

    void setField(const std::string& name, const char* value) {
        fields_[name] = std::any(std::string(value));
    }

    template<typename T>
    void setField(const std::string& name, T&& value) {
        using Decayed = std::decay_t<T>;
        // 支持 std::any 作为参数，允许直接存储支持类型
        if constexpr (std::is_same_v<Decayed, std::any>) {
            const std::any& any_val = value;
            if (is_supported_any_type(any_val)) {
                fields_[name] = any_val;
            } else {
                throw std::runtime_error("Unsupported field type");
            }
        } else if constexpr (is_supported_type<Decayed>()) {
            fields_[name] = std::any(std::forward<T>(value));
        } else {
            throw std::runtime_error("Unsupported field type");
        }
    }

    void removeField(const std::string& name) {
        fields_.erase(name);
    }

    void removeAllFields() {
        fields_.clear();
    }

    std::vector<std::string> getFieldNames() const {
        std::vector<std::string> names;
        names.reserve(fields_.size());
        for (const auto& [key, _] : fields_) {
            names.push_back(key);
        }
        return names;
    }

    bool hasField(const std::string& name) const {
        return fields_.find(name) != fields_.end();
    }

    std::optional<std::any> getField(const std::string& name) const {
        auto it = fields_.find(name);
        if (it != fields_.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    const std::string& getName() const {
        return name_;
    }

    bool isEqual(const Structure& other) const {
        if (name_ != other.name_) return false;
        if (fields_.size() != other.fields_.size()) return false;
        for (const auto& [key, value] : fields_) {
            auto it = other.fields_.find(key);
            if (it == other.fields_.end()) return false;
            if (!anyEquals(value, it->second)) return false;
        }
        return true;
    }

private:
    template<typename T>
    static constexpr bool is_supported_type() {
        return std::is_same_v<T, jst_bool> ||
               std::is_same_v<T, jst_int> ||
               std::is_same_v<T, jst_int64> ||
               std::is_same_v<T, jst_string> ||
               std::is_same_v<T, jst_float> ||
               std::is_same_v<T, jst_double> ||
               std::is_same_v<T, jst_range> ||
               std::is_same_v<T, int> ||
               std::is_same_v<T, float> ||
               std::is_same_v<T, double> ||
               std::is_same_v<T, std::string> ||
               std::is_same_v<T, bool>;
    }

    static bool is_supported_any_type(const std::any& value) {
        return value.type() == typeid(jst_bool) ||
               value.type() == typeid(jst_int) ||
               value.type() == typeid(jst_int64) ||
               value.type() == typeid(jst_string) ||
               value.type() == typeid(jst_float) ||
               value.type() == typeid(jst_double) ||
               value.type() == typeid(jst_range) ||
               value.type() == typeid(int) ||
               value.type() == typeid(float) ||
               value.type() == typeid(double) ||
               value.type() == typeid(std::string) ||
               value.type() == typeid(bool);
    }

    static bool anyEquals(const std::any& a, const std::any& b) {
        if (a.type() != b.type()) return false;
        if (a.type() == typeid(int)) {
            return std::any_cast<int>(a) == std::any_cast<int>(b);
        }
        if (a.type() == typeid(float)) {
            return std::any_cast<float>(a) == std::any_cast<float>(b);
        }
        if (a.type() == typeid(double)) {
            return std::any_cast<double>(a) == std::any_cast<double>(b);
        }
        if (a.type() == typeid(std::string)) {
            return std::any_cast<std::string>(a) == std::any_cast<std::string>(b);
        }
        if (a.type() == typeid(jst_bool)) {
            return std::any_cast<jst_bool>(a) == std::any_cast<jst_bool>(b);
        }
        if (a.type() == typeid(jst_int)) {
            return std::any_cast<jst_int>(a) == std::any_cast<jst_int>(b);
        }
        if (a.type() == typeid(jst_int64)) {
            return std::any_cast<jst_int64>(a) == std::any_cast<jst_int64>(b);
        }
        if (a.type() == typeid(jst_string)) {
            return std::any_cast<jst_string>(a) == std::any_cast<jst_string>(b);
        }
        if (a.type() == typeid(jst_float)) {
            return std::any_cast<jst_float>(a) == std::any_cast<jst_float>(b);
        }
        if (a.type() == typeid(jst_double)) {
            return std::any_cast<jst_double>(a) == std::any_cast<jst_double>(b);
        }
        if (a.type() == typeid(jst_range)) {
            return std::any_cast<jst_range>(a) == std::any_cast<jst_range>(b);
        }
        if (a.type() == typeid(bool)) {
            return std::any_cast<bool>(a) == std::any_cast<bool>(b);
        }
        return false;
    }

    std::string name_;
    std::unordered_map<std::string, std::any> fields_;
};
}

#endif //JST_STRUCTURE_H
