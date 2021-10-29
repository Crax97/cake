// This file was generated automatically using the katsu code analyzer or whatever they are called
// https://github.com/crax97/katsu 

#include "object/descriptor.h"
#include "object/object.h"
#include "object/fields.h"

namespace spectacle {
class stage_descriptor : public descriptor{
  public:
  stage_descriptor() : descriptor("stage", nullptr) {
    field_adder<spectacle::stage,decltype(spectacle::stage::m_actor_list)>()(m_fields, "m_actor_list", &stage::m_actor_list);

    }
    [[nodiscard]] const std::type_info& get_typeinfo() const override {
        return typeid(spectacle::stage);
    }
};
}

descriptor* spectacle::stage::get_descriptor() const {
  static spectacle::stage_descriptor desc;
  return &desc;
}
