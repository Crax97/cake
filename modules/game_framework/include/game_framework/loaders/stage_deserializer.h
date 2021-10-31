//
// Created by crax on 10/28/21.
//
#pragma once

#include <string>
#include <string_view>
#include <memory>

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "game_framework/stage.h"

/*
scene file grammar
scene_decl = "scene" [
    decl_list
]
decl_list = "" | decl decl_list
decl = word parameter_list sub_decls
sub_decls = "" | [ decl_list ]
parameter_list = "" | word=parameter parameter_list
parameter = number_array | string | bool

number_array = number number_array | number ""
string = 'word'
bool = true | false

word = // any combination of alphanumeric characters, beginning with a letter
*/

namespace gameframework {
    struct token;
    class stage_deserializer {
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

        std::unique_ptr<spectacle::stage> m_stage;
        int m_tabs = 0;
        void begin(class miku_tokenizer& stage_tokenizer);
        section_type next_section(class miku_tokenizer &tok);
        void end(class miku_tokenizer& stage_tokenizer);
        [[nodiscard]] bool has_section(class miku_tokenizer& tok) const;
        void deserialize_actor(class miku_tokenizer& tokenizer1);
        void read_param(actor_descriptor& actor_desc, const token& token, class miku_tokenizer& tok);
        double read_number(class miku_tokenizer &tok);
        glm::vec3 read_vec3(class miku_tokenizer &tok);
        glm::quat read_rotation(class miku_tokenizer & tok);

        std::shared_ptr<spectacle::actor> actor_from_description(actor_descriptor& desc);
        public:
        explicit stage_deserializer() : m_stage(std::make_unique<spectacle::stage>()) { }
        void deserialize(std::istream& stage_stream);
        [[nodiscard]] std::unique_ptr<spectacle::stage>&& get_stage();
        bool good();

        void read_components(std::shared_ptr<spectacle::actor> &actor, class miku_tokenizer & tok);

        void read_component(std::string_view component_name, std::shared_ptr<spectacle::actor> &actor_ptr, class miku_tokenizer &tok);
    };
}