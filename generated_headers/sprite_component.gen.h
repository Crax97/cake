// This file was generated automatically using the katsu code analyzer or whatever they are called
// https://github.com/crax97/katsu 

#include "object/fields.h"
#include "object/macros.h"

namespace gameframework {
    CORE_BEGIN_DESCRIPTOR(sprite_component)
            CORE_REFLECT_MEMBER(m_texture)
            CORE_REFLECT_MEMBER(m_sprite_scale)


    CORE_END_DESCRIPTOR()
}

CORE_IMPLEMENT_FUNCTIONS(gameframework::sprite_component)
