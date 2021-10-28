#pragma once

#include "descriptor.h"
#include "field.h"
#include "serializer.h"
#include <cassert>

#include <list>
#include <memory>
#include <string>
#include <typeinfo>


class object {
public:
  [[nodiscard]] virtual descriptor *get_descriptor() const = 0;
  virtual void serialize(class serializer &serializer) {
    serializer.begin_class(*get_descriptor(), this);
    for (auto &field : get_descriptor()->get_fields()) {
      serializer.serialize_field(*field, this);
    }
    serializer.end_class(*get_descriptor(), this);
  }

  [[nodiscard]] virtual std::string to_string() const;

};
std::ostream& operator<<(std::ostream& stream, const object& obj);