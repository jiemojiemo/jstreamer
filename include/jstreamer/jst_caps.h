//
// Created by user on 7/31/25.
//

#ifndef JST_CAPS_H
#define JST_CAPS_H
#include "jstreamer/jst_structure.h"
namespace jstreamer {
class Caps {
public:
  Caps() = default;
  explicit Caps(const char* mimeType) {
    auto s = Structure(mimeType);
    structures_.emplace_back(std::move(s));
  }
  size_t getSize() const {
    return structures_.size(); // Placeholder implementation, should return the actual size of the caps
  }

  Structure* getStructure(int index) {
    if (index >= structures_.size()) {
      return nullptr;
    }

    return &structures_[index];
  }

  void appendStructure(Structure structure) {
    structures_.emplace_back(std::move(structure));
  }

  bool isAny() const {
    return is_any_;
  }

  static Caps any() {
    Caps caps;
    caps.is_any_ = true;
    return caps;
  }


private:
  std::vector<Structure> structures_;
  bool is_any_{false};
};
}

#endif //JST_CAPS_H
