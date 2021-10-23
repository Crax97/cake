#pragma once

#include <cassert>
#include <list>
#include <memory>
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
  const std::string &get_name() const { return m_field_name; }
  const std::list<std::unique_ptr<field>> &get_fields() const {
    return m_fields;
  }
};

template <typename T> descriptor *get_descriptor_typed() {
  assert(false && "This shouldn't be reached");
  return nullptr;
}