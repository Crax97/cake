//
// Created by crax on 10/29/21.
//

#include "object/object.h"
#include "serializers/string_serializer.h"

std::string object::to_string() const {
    string_serializer ser;
    this->serialize(ser);
    return ser.get_str();
}

void object::serialize(serializer &serializer) const  {
    serializer.begin_section(get_descriptor()->get_name());
    for (auto &field : get_descriptor()->get_fields()) {
        serializer.add_parameter(field.first, '"' + field.second->to_string(this) + '"');
    }
    serializer.end_section(get_descriptor()->get_name());
}


std::ostream &operator<<(std::ostream &stream, const object& obj) {
    stream << obj.to_string();
    return stream;
}


