//
// Created by user on 6/27/25.
//

#ifndef JST_I_ELEMENT_H
#define JST_I_ELEMENT_H

namespace jstreamer{
class IElement{
public:
  virtual ~IElement() = default;

  virtual std::string getName() const = 0;
  virtual void setName(const std::string &name) = 0;
};
}

#endif //JST_I_ELEMENT_H
