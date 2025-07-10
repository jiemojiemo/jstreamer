//
// Created by user on 7/9/25.
//

#ifndef JST_STRUCTURE_H
#define JST_STRUCTURE_H

#include <vector>
#include <string>
#include <unordered_map>
#include <any>
#include <optional>

namespace jstreamer {
class Structure {
public:
    explicit Structure(std::string name) : name_(std::move(name)) {}

    template<typename T>
    void setField(const std::string& name, T&& value) {
        fields_[name] = std::any(std::forward<T>(value));
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
        // 可扩展其它类型
        return false;
    }

    std::string name_;
    std::unordered_map<std::string, std::any> fields_;
};
}

#endif //JST_STRUCTURE_H
