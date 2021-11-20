#pragma once

#include "game_framework/component.h"
#include "game_framework/actor.h"
#include "game_framework/resources/script_resource.h"

#include "object/macros.h"

#include <list>
#include <memory>

namespace spectacle {
    class actor;
}

namespace gameframework {

class sprite_component_descriptor : public descriptor {
private:
    game_framework::script_resource& m_resource;
public:
    const std::type_info &get_typeinfo() const override;
    explicit sprite_component_descriptor(game_framework::script_resource& resource);


};

class REFLECT() script_component : public spectacle::component {
    CORE_GENERATE_BODY(sprite_component)
    private:
        game_framework::script_resource m_script;
        std::unique_ptr<descriptor> m_descriptor;
    public:
        explicit script_component(spectacle::actor &owner) noexcept;

        void begin_play() noexcept override;
        void update(float deltatime) noexcept override;
        void after_update(float delta_time) noexcept override;
        std::shared_ptr<spectacle::component> clone(spectacle::actor& new_owner) const noexcept override;

        ~script_component();
    };
} // namespace gameframework