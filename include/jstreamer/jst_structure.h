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
    template<typename T>
    void setField(const std::string& name, T&& value) {
        fields_[name] = std::any(std::forward<T>(value));
        updateFieldNames();
    }

    const std::vector<std::string>& getFieldNames() const {
        return fieldNames_;
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

private:
    void updateFieldNames() {
        fieldNames_.clear();
        for (const auto& kv : fields_) {
            fieldNames_.push_back(kv.first);
        }
    }

    std::unordered_map<std::string, std::any> fields_;
    std::vector<std::string> fieldNames_;
};
}

#endif //JST_STRUCTURE_H
