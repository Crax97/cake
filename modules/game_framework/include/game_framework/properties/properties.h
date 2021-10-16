#pragma once
#include "game_framework/properties/property.h"
#include "texture.h"
#include <cassert>
#include <memory>

namespace gameframework {

class texture_property_interface {

public:
  virtual std::shared_ptr<renderer::texture> &get_texture(void *self) = 0;
};

template <typename Klass>
class texture_property final : public property_system::property,
                               public texture_property_interface {
private:
  std::shared_ptr<renderer::texture> Klass::*m_ref;

protected:
  virtual void set_impl(void *base, void *data, size_t data_size) override {
    assert(sizeof(renderer::texture) == data_size);
    assert(base != nullptr);
    auto &self = *static_cast<Klass *>(base);
    // self.*m_ref = *static_cast<renderer::texture *>(data);
  }
  virtual void *get_impl(void *base, size_t data_size) override {
    assert(sizeof(renderer::texture) == data_size);
    assert(base != nullptr);
    auto &self = *static_cast<Klass *>(base);
    return nullptr;
  }

public:
  explicit texture_property(
      std::shared_ptr<renderer::texture> Klass::*ref) noexcept
      : m_ref(ref) {}
  std::shared_ptr<renderer::texture> &get_texture(void *self) override {
    Klass &instance = *static_cast<Klass *>(self);
    return instance.*m_ref;
  }
};
} // namespace gameframework