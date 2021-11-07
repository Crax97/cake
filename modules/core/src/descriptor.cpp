//
// Created by crax on 10/30/21.
//

#include "object/descriptor.h"
#include "object/field.h"
#include "object/function.h"

std::optional<field *> descriptor::find_field(std::string_view field_name) const {
    auto field_found = m_fields.find(std::string(field_name));
    return (field_found == m_fields.end()) ? std::nullopt : std::optional<field*>(field_found->second.get());
}

std::optional<method *> descriptor::find_method(std::string_view method_name) const {
    auto method_found = m_methods.find(std::string(method_name));
    return (method_found == m_methods.end()) ? std::nullopt : std::optional<method*>(method_found->second.get());
}

const std::unordered_map<std::string, std::unique_ptr<field>> &descriptor::get_fields() const {
    return m_fields;
}

const std::unordered_map<std::string, std::unique_ptr<method>> &descriptor::get_methods() const {
    return m_methods;
}

descriptor::~descriptor() = default;
