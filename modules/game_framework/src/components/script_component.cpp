#include "game_framework/components/script_component.h"
#include "game_framework/initialization_object.h"
#include "game_framework/loaders/texture_loader.h"
#include "luanatic/script.h"

#include "game_framework/actor.h"

#include <memory>


gameframework::script_component::script_component(spectacle::actor &owner) noexcept
        : component(owner) {
    m_descriptor = std::make_unique<sprite_component_descriptor>(m_script);
}

std::shared_ptr<spectacle::component>
gameframework::script_component::clone(spectacle::actor &new_owner) const noexcept {
    MIKU_ASSERT(false && "IMPLEMENT ME BITCH");
    return nullptr;
}

void gameframework::script_component::begin_play() noexcept {
    if(!m_script) return;
    auto& script = m_script.get_script();
    script->call("begin_play");
}

void gameframework::script_component::update(float delta_time) noexcept {
    if(!m_script) return;
    auto& script = m_script.get_script();
    script->call("update", delta_time);
}

void gameframework::script_component::after_update(float delta_time) noexcept {
    if(!m_script) return;
    auto& script = m_script.get_script();
    script->call("after_update", delta_time);
}

gameframework::script_component::~script_component() = default;


template<>
descriptor* get_descriptor_typed<gameframework::script_component>() {
    MIKU_ASSERT(false && "Please don't use get_descriptor_typed for script_component");
    return nullptr;
}
namespace gameframework {
    sprite_component_descriptor::sprite_component_descriptor(game_framework::script_resource &resource)
            : descriptor("script_descriptor", nullptr),
              m_resource(resource) {
        field_adder<gameframework::script_component, game_framework::script_resource>()(m_fields, "script",
                                                                                        &gameframework::script_component::m_script);
    }

    descriptor *gameframework::script_component::get_descriptor() const {
        return m_descriptor.get();
    }

    const std::type_info &sprite_component_descriptor::get_typeinfo() const {
        return typeid(luanatic::script);
    }
}