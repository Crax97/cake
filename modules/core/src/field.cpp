#include "object/field.h"

#include "glm/glm.hpp"

template<>
void do_visit<char const*>(char const* const& value, field_visitor & visitor) {
    visitor.visit_string_property(value);
}

field::~field() = default;
