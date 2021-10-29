#pragma once

#include "descriptor.h"
#include "serializer.h"
#include "fields.h"
#include <cassert>

#include <list>
#include <memory>
#include <string>
#include <typeinfo>


class object {
public:
  [[nodiscard]] virtual descriptor *get_descriptor() const = 0;
  virtual void serialize(class serializer &serializer) const {
    serializer.begin_section("actor");
    for (auto &field : get_descriptor()->get_fields()) {
      serializer.add_parameter(field->get_name(), field->to_string(this));
    }
    serializer.end_section("actor");
  }

  [[nodiscard]] virtual std::string to_string() const;

};
std::ostream& operator<<(std::ostream& stream, const object& obj);