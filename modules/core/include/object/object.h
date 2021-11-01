#pragma once

#include "descriptor.h"
#include "serializer.h"

#include <list>
#include <memory>
#include <string>
#include <typeinfo>


class object {
public:
  [[nodiscard]] virtual descriptor *get_descriptor() const = 0;
  virtual void serialize(class serializer &serializer) const;
  [[nodiscard]] virtual std::string to_string() const;

};
std::ostream& operator<<(std::ostream& stream, const object& obj);