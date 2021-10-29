//
// Created by crax on 10/29/21.
//

#include <object/object.h>
#include "serializers/string_serializer.h"
#include "object/field.h"

std::string object::to_string() const {
    string_serializer ser;
    this->serialize(ser);
    return ser.get_str();
}

std::ostream &operator<<(std::ostream &stream, const object& obj) {
    stream << obj.to_string();
    return stream;
}


