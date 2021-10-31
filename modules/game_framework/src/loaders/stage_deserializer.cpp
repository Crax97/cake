//
// Created by crax on 10/28/21.
//

#include "game_framework/loaders/stage_deserializer.h"
#include "game_framework/loaders/miku_tokenizer.h"
#include "game_framework/actor.h"
#include "game_framework/component.h"


#include <cassert>
#include <istream>
#include <memory>
#include <game_framework/components/component_repository.h>

void gameframework::stage_deserializer::begin(miku_tokenizer& tok) {
    auto section = tok.consume(token_type::id);
    assert(section.spelling == "scene");
    tok.consume(token_type::left_square_paren);
}

void gameframework::stage_deserializer::end(miku_tokenizer& tok) {
    tok.consume(token_type::right_square_paren);
}

void gameframework::stage_deserializer::deserialize(std::istream &stage_stream) {
    miku_tokenizer tok(stage_stream);
    tok.tokenize();
    begin(tok);
    while(has_section(tok)) {
        section_type type = next_section(tok);
        switch(type) {
            case section_type::actor:
                deserialize_actor(tok);
                break;
        }
    }
    end(tok);
}

bool gameframework::stage_deserializer::has_section(miku_tokenizer& tok) const {
    return tok.peek().type == token_type::id;
}
gameframework::stage_deserializer::section_type gameframework::stage_deserializer::next_section(miku_tokenizer& tok) {
    auto id = tok.consume(token_type::id);
    if(id.spelling == "actor") {
        return section_type::actor;
    }
    assert(false);
}

std::shared_ptr<spectacle::actor> gameframework::stage_deserializer::actor_from_description(gameframework::stage_deserializer::actor_descriptor& desc) {

    std::shared_ptr<spectacle::actor> actor;
    if(desc.prototype.empty()) {
        actor = std::make_shared<spectacle::actor>();
    } else {
        // actor = std::make_shared<spectacle::actor>();
    }

    for(const auto& [key, value] : desc.values) {
        if(key == "name") {
            actor->set_name(value);
            continue;
        }
        auto field = actor->get_descriptor()->find_field(key);
        if(field) {
            field.value()->set_from_string(actor.get(), value);
        }
    }

    return actor;
}

void gameframework::stage_deserializer::deserialize_actor(miku_tokenizer& tok) {
    tok.consume(token_type::left_square_paren);
    auto desc = actor_descriptor { };
    std::shared_ptr<spectacle::actor> actor;
    while(tok.peek().type != token_type::right_square_paren) {
        auto id = tok.consume(token_type::id);
        if(tok.peek().type == token_type::equal) {
            // It's a parameter;
            tok.consume(token_type::equal);
            read_param(desc, id, tok);
        } else if(tok.peek().type != token_type::right_square_paren) {
            if(!actor) {
                actor = actor_from_description(desc);
            }
            read_component(id.spelling, actor, tok);
            m_stage->add_new_actor(actor);
        }
    }
    tok.consume(token_type::right_square_paren);
}


void gameframework::stage_deserializer::read_param(actor_descriptor& actor_desc, const token& key, miku_tokenizer& tok) {
    if(key.spelling == "prototype") {
        auto base_entity = tok.consume(token_type::string).spelling;
        actor_desc.prototype = base_entity;
    } else {
        auto value = tok.consume(token_type::string).spelling;
        actor_desc.values[key.spelling] = value;
    }
}

bool gameframework::stage_deserializer::good() {
    return true;
}

std::unique_ptr<spectacle::stage> &&gameframework::stage_deserializer::get_stage() {
    return std::move(m_stage);
}

void gameframework::stage_deserializer::read_components(std::shared_ptr<spectacle::actor> &actor,
                                                        miku_tokenizer &tok) {

}

void
gameframework::stage_deserializer::read_component(std::string_view component_name, std::shared_ptr<spectacle::actor> &actor_ptr,
                                                  miku_tokenizer &tok) {
    std::shared_ptr<spectacle::component> component_ptr = actor_ptr->find_component_by_name(component_name);
    if(!component_ptr) {
        auto component_maybe = component_repository::the().construct_component(std::string(component_name), *actor_ptr);
        assert(component_maybe.has_value());
        component_ptr = component_maybe.value();
    }
    tok.consume(token_type::left_square_paren);
    while(tok.peek().type != token_type::right_square_paren) {
        std::string prop_name = tok.consume(token_type::id).spelling;
        tok.consume(token_type::equal);
        std::string prop_value = tok.consume(token_type::string).spelling;

        auto field = component_ptr->get_descriptor()->find_field(prop_name);
        if(field.has_value()) {
            field.value()->set_from_string(component_ptr.get(), std::string(prop_value));
        }
    }
    tok.consume(token_type::right_square_paren);
    actor_ptr->add_new_component(component_ptr);
}

