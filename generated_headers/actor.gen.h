// This file was generated automatically using the katsu code analyzer or whatever they are called
// https://github.com/crax97/katsu 

#include "object/fields.h"
#include "object/macros.h"

namespace spectacle {
    CORE_BEGIN_DESCRIPTOR(actor)
            CORE_REFLECT_MEMBER(m_transform)
            CORE_REFLECT_MEMBER(m_name)
            CORE_REFLECT_MEMBER(is_enabled)
            CORE_REFLECT_MEMBER(tick_enabled)

    CORE_END_DESCRIPTOR()
}

CORE_IMPLEMENT_FUNCTIONS(spectacle::actor)
