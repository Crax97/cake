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
  virtual descriptor *get_descriptor() = 0;
  virtual void serialize(class serializer &serializer) {
    serializer.begin_class(*get_descriptor(), this);
    for (auto &field : get_descriptor()->get_fields()) {
      serializer.serialize_field(*field, this);
    }
    serializer.end_class(*get_descriptor(), this);
  }
};