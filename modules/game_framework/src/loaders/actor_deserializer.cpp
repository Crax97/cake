//
// Created by crax on 10/31/21.
//

#include "game_framework/loaders/actor_deserializer.h"
#include "game_framework/components/component_repository.h"
#include "game_framework/actor.h"
#include "game_framework/component.h"

#include "game_framework/loaders/miku_tokenizer.h"
#include "object/field.h"
#include "object/descriptor.h"

void gameframework::actor_deserializer::deserialize_actor(miku_tokenizer& tok) {
    tok.consume(token_type::left_square_paren);
    while(tok.peek().type != token_type::right_square_paren) {
        auto id = tok.consume(token_type::id);
        if(tok.peek().type == token_type::equal) {
            tok.consume(token_type::equal);
            auto next_val = tok.consume(token_type::string).spelling;

            auto actor_field = m_actor->get_descriptor()->find_field(id.spelling);
            if(actor_field) {
                actor_field.value()->set_from_string(m_actor.get(), next_val);
            }
        } else {
            read_component(id.spelling, m_actor, tok);
        }
    }
    tok.consume(token_type::right_square_paren);
}

gameframework::actor_deserializer::actor_deserializer()
    : m_actor(std::make_unique<spectacle::actor>()) {
    m_actor->enable_prototype();
}

void gameframework::actor_deserializer::deserialize(std::istream &stage_stream) {
    auto tokenizer = miku_tokenizer(stage_stream);
    begin(tokenizer);
    deserialize_actor(tokenizer);
    end(tokenizer);
}

void gameframework::actor_deserializer::begin(gameframework::miku_tokenizer &stage_tokenizer) {
    assert(stage_tokenizer.consume(token_type::id).spelling == "actor");
    stage_tokenizer.consume(token_type::left_square_paren);
}

void gameframework::actor_deserializer::end(gameframework::miku_tokenizer &stage_tokenizer) {
    stage_tokenizer.consume(token_type::right_square_paren);
}

bool gameframework::actor_deserializer::good() {
    return false;
}

void gameframework::actor_deserializer::read_component(std::string_view component_name,
                                                       std::shared_ptr<spectacle::actor> &actor_ptr,
                                                       gameframework::miku_tokenizer &tok) {
    auto component_maybe = component_repository::the().construct_component(std::string(component_name), *actor_ptr);
    assert(component_maybe.has_value());
    auto component_ptr = component_maybe.value();
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
