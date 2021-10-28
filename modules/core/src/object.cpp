//
// Created by crax on 10/29/21.
//

#include <object/object.h>
#include "object/field.h"

std::string object::to_string() const {
        auto* tthis = this;
        std::stringstream stream;
        std::string class_name = get_descriptor()->get_name();
        stream << class_name << " @0x" << std::hex << *reinterpret_cast<size_t*>(&tthis) << std::dec << "\n";
        auto& fields = get_descriptor()->get_fields();
        if(!fields.empty()) {
            stream << "{\n";
            for(const auto& field : fields) {
                stream << "\t" << field->get_name() << " : " << field->to_string(static_cast<void*>(const_cast<object*>(this))) << "\n";
            }
            stream << "}";
        }
        return stream.str();
    }

std::ostream &operator<<(std::ostream &stream, const object& obj) {
    stream << obj.to_string();
    return stream;
}


