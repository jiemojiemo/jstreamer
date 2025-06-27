//
// Created by user on 6/27/25.
//

#ifndef JST_BASE_ELEMENT_H
#define JST_BASE_ELEMENT_H
#include "jstreamer/jst_i_element.h"
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

private:
  std::string name_;
};
}

#endif //JST_BASE_ELEMENT_H
