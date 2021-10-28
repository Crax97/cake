// This file was generated automatically using the katsu code analyzer or whatever they are called
// https://github.com/crax97/katsu 

#include "object/descriptor.h"
#include "object/object.h"
#include "object/pointer_field.h"

namespace gameframework {
class sprite_component_descriptor : public descriptor{
  public:
  sprite_component_descriptor() : descriptor("sprite_component", nullptr) {
    field_adder<gameframework::sprite_component,decltype(gameframework::sprite_component::m_texture)>()(m_fields, "m_texture", &sprite_component::m_texture);
    field_adder<gameframework::sprite_component,decltype(gameframework::sprite_component::m_sprite_scale)>()(m_fields, "m_sprite_scale", &sprite_component::m_sprite_scale);

    }
    [[nodiscard]] const std::type_info& get_typeinfo() const override {
        return typeid(gameframework::sprite_component);
    }
};
}

descriptor* gameframework::sprite_component::get_descriptor() {
  static gameframework::sprite_component_descriptor desc;
  return &desc;
}
