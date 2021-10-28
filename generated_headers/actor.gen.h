// This file was generated automatically using the katsu code analyzer or whatever they are called
// https://github.com/crax97/katsu 

#include "object/descriptor.h"
#include "object/object.h"
#include "object/pointer_field.h"

namespace spectacle {
class actor_descriptor : public descriptor{
  public:
  actor_descriptor() : descriptor("actor", nullptr) {
    field_adder<spectacle::actor,decltype(spectacle::actor::m_transform)>()(m_fields, "m_transform", &actor::m_transform);
    field_adder<spectacle::actor,decltype(spectacle::actor::is_enabled)>()(m_fields, "is_enabled", &actor::is_enabled);
    field_adder<spectacle::actor,decltype(spectacle::actor::tick_enabled)>()(m_fields, "tick_enabled", &actor::tick_enabled);

    }
    [[nodiscard]] const std::type_info& get_typeinfo() const override {
        return typeid(spectacle::actor);
    }
};
}

descriptor* spectacle::actor::get_descriptor() {
  static spectacle::actor_descriptor desc;
  return &desc;
}
