#pragma once

#include <cassert>
#include <list>
#include <memory>
#include <optional>
#include <string>

class field;

class descriptor {
private:
  std::string m_field_name;
  descriptor *m_parent{nullptr};

protected:
  std::list<std::unique_ptr<field>> m_fields;

public:
  descriptor(const std::string &field_name, descriptor *parent)
      : m_field_name(field_name), m_parent(parent) {}
  descriptor *parent() { return m_parent; }
  [[nodiscard]] virtual const std::type_info& get_typeinfo() const = 0;
  [[nodiscard]] const std::string &get_name() const { return m_field_name; }
    [[nodiscard]] const std::list<std::unique_ptr<field>> &get_fields() const {
        return m_fields;
    }
    [[nodiscard]] std::optional<field*> find_field(std::string_view field_name) const;
};

template <typename T> descriptor *get_descriptor_typed() {
  assert(false && "This shouldn't be reached");
  return nullptr;
}