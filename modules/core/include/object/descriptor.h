#pragma once

#include "logging/logger.h"
#include "fields.h"

#include <unordered_map>
#include <memory>
#include <optional>
#include <string>
#include <utility>


class descriptor {
private:
  std::string m_field_name;
  descriptor *m_parent{nullptr};

protected:
    std::unordered_map<std::string, std::unique_ptr<field>> m_fields { };
    std::unordered_map<std::string, std::unique_ptr<method>> m_methods { };
    std::unordered_map<std::string, std::unique_ptr<callable>> m_bound_functions { };

public:
  descriptor(std::string field_name, descriptor *parent)
      : m_field_name(std::move(field_name)), m_parent(parent) {}
  descriptor *parent() { return m_parent; }
      [[nodiscard]] virtual const std::type_info& get_typeinfo() const = 0;
      [[nodiscard]] const std::string &get_name() const { return m_field_name; }
    [[nodiscard]] const std::unordered_map<std::string, std::unique_ptr<field>> &get_fields() const;
    [[nodiscard]] std::optional<field*> find_field(std::string_view field_name) const;
    [[nodiscard]] const std::unordered_map<std::string, std::unique_ptr<method>> &get_methods() const;
    [[nodiscard]] std::optional<method*> find_method(std::string_view method_name) const;
    ~descriptor();
};

template <typename T> descriptor *get_descriptor_typed() {
  MIKU_ASSERT(false && "This shouldn't be reached");
  return nullptr;
}