//
// Created by crax on 10/30/21.
//

#include "object/descriptor.h"
#include "object/field.h"

std::optional<field *> descriptor::find_field(std::string_view field_name) const {
    for(auto& field : m_fields) {
        if(field->get_name() == field_name) {
            return field.get();
        }
    }
    return {};
}
