//
// Created by crax on 10/31/21.
//

#pragma once

#include "game_framework/actor.h"
#include <glm/glm.hpp>

namespace gameframework {
    class actor_deserializer {
        struct token;

    private:
        enum class section_type {
            actor,
        };

        struct actor_descriptor {
            std::string name;
            std::string prototype;
            glm::vec3 location;
            glm::quat rotation;
            glm::vec3 scale;
        };

        std::shared_ptr <spectacle::actor> m_actor;
        int m_tabs = 0;

        section_type next_section(class miku_tokenizer &tok);

        void begin(class miku_tokenizer &stage_tokenizer);
        void end(class miku_tokenizer &stage_tokenizer);


        void deserialize_actor(class miku_tokenizer &tokenizer1);

    public:
        explicit actor_deserializer();

        void deserialize(std::istream &stage_stream);

        [[nodiscard]] std::shared_ptr <spectacle::actor> &&get_actor() {
            return std::move(m_actor);
        }

        bool good();

        void read_component(std::string_view component_name, std::shared_ptr <spectacle::actor> &actor_ptr,
                            class miku_tokenizer &tok);
    };
}