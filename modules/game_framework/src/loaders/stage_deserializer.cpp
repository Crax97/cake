//
// Created by crax on 10/28/21.
//

#include "game_framework/loaders/stage_deserializer.h"
#include "game_framework/components/component_repository.h"
#include "game_framework/actor.h"
#include "game_framework/component.h"

#include <cassert>
#include <istream>
#include <memory>

void gameframework::stage_deserializer::begin(tokenizer& tok) {
    auto section = tok.consume(tokenizer::token_type::id);
    assert(section.spelling == "scene");
    tok.consume(tokenizer::token_type::left_square_paren);
}

void gameframework::stage_deserializer::end(tokenizer& tok) {
    tok.consume(tokenizer::token_type::right_square_paren);
}

void gameframework::stage_deserializer::deserialize(std::istream &stage_stream) {
    tokenizer tok(stage_stream);
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

bool gameframework::stage_deserializer::has_section(tokenizer& tok) const {
    return tok.peek().type == tokenizer::token_type::id;
}
gameframework::stage_deserializer::section_type gameframework::stage_deserializer::next_section(gameframework::stage_deserializer::tokenizer& tok) {
    auto id = tok.consume(tokenizer::token_type::id);
    if(id.spelling == "actor") {
        return section_type::actor;
    }
    assert(false);
}

std::shared_ptr<spectacle::actor> gameframework::stage_deserializer::actor_from_description(gameframework::stage_deserializer::actor_descriptor& desc) {
    assert(!desc.prototype.empty());
    auto actor = std::make_shared<spectacle::actor>();
    actor->set_location(desc.location);
    actor->set_rotation(desc.rotation);
    actor->set_scale(desc.scale);
    return actor;
}

void gameframework::stage_deserializer::deserialize_actor(gameframework::stage_deserializer::tokenizer& tok) {
    tok.consume(tokenizer::token_type::left_square_paren);
    auto desc = actor_descriptor { };
    std::shared_ptr<spectacle::actor> actor;
    while(tok.peek().type != tokenizer::token_type::right_square_paren) {
        auto id = tok.consume(tokenizer::token_type::id);
        if(tok.peek().type == tokenizer::token_type::equal) {
            // It's a parameter;
            tok.consume(tokenizer::token_type::equal);
            read_param(desc, id, tok);
        } else if(tok.peek().type != tokenizer::token_type::right_square_paren) {
            if(!actor) {
                actor = actor_from_description(desc);
            }
            read_component(id.spelling, actor, tok);
            m_stage->add_new_actor(actor);
        }
    }
    tok.consume(tokenizer::token_type::right_square_paren);
}


void gameframework::stage_deserializer::read_param(actor_descriptor& actor_desc, const tokenizer::token& key, tokenizer& tok) {
    if(key.spelling == "prototype") {
        auto base_entity = tok.consume(tokenizer::token_type::string).spelling;
        actor_desc.prototype = base_entity;
    } else if(key.spelling == "location") {
        actor_desc.location = read_vec3(tok);
    } else if(key.spelling == "rotation") {
        actor_desc.rotation = read_rotation(tok);
    } else if(key.spelling == "scale") {
        actor_desc.scale = read_vec3(tok);
    }
}

double gameframework::stage_deserializer::read_number(gameframework::stage_deserializer::tokenizer &tok) {
    auto num = tok.consume(tokenizer::token_type::number);
    return std::stod(num.spelling);
}

glm::vec3 gameframework::stage_deserializer::read_vec3(gameframework::stage_deserializer::tokenizer &tok) {
    tok.consume(tokenizer::token_type::left_square_paren);
    glm::vec3 vec;
    vec.x = read_number(tok);
    tok.consume(tokenizer::token_type::comma);
    vec.y = read_number(tok);
    tok.consume(tokenizer::token_type::comma);
    vec.z = read_number(tok);
    tok.consume(tokenizer::token_type::right_square_paren);
    return vec;
}

glm::quat gameframework::stage_deserializer::read_rotation(gameframework::stage_deserializer::tokenizer &tok) {
    tok.consume(tokenizer::token_type::left_square_paren);
    glm::quat rot;
    rot.x = read_number(tok);
    tok.consume(tokenizer::token_type::comma);
    rot.y = read_number(tok);
    tok.consume(tokenizer::token_type::comma);
    rot.z = read_number(tok);
    tok.consume(tokenizer::token_type::comma);
    rot.w = read_number(tok);
    tok.consume(tokenizer::token_type::right_square_paren);
    return rot;
}

bool gameframework::stage_deserializer::good() {
    return true;
}

std::unique_ptr<spectacle::stage> &&gameframework::stage_deserializer::get_stage() {
    return std::move(m_stage);
}

void gameframework::stage_deserializer::read_components(std::shared_ptr<spectacle::actor> &actor,
                                                        gameframework::stage_deserializer::tokenizer &tok) {

}

void
gameframework::stage_deserializer::read_component(std::string_view component_name, std::shared_ptr<spectacle::actor> &actor_ptr,
                                                  gameframework::stage_deserializer::tokenizer &tok) {
    auto component_maybe = component_repository::the().construct_component(std::string(component_name), *actor_ptr);
    assert(component_maybe.has_value());
    auto component_ptr = component_maybe.value();
    tok.consume(tokenizer::token_type::left_square_paren);
    while(tok.peek().type != tokenizer::token_type::right_square_paren) {
        std::string prop_name = tok.consume(tokenizer::token_type::id).spelling;
        tok.consume(tokenizer::token_type::equal);
        std::string prop_value = tok.consume(tokenizer::token_type::string).spelling;

        auto field = component_ptr->get_descriptor()->find_field(prop_name);
        if(field.has_value()) {
            field.value()->set_from_string(component_ptr.get(), std::string(prop_value));
        }
    }
    tok.consume(tokenizer::token_type::right_square_paren);
    actor_ptr->add_new_component(component_ptr);
}


void gameframework::stage_deserializer::tokenizer::tokenize() {
    while(m_stream.good() && m_stream.peek() != EOF) {
        ignore_characters();
        auto ch = m_stream.peek();
        if(m_stream.peek() == '[') {
            m_stream.get();
            push( { token_type::left_square_paren, "["});
        } else if(m_stream.peek() == ']') {
            m_stream.get();
            push( { token_type::right_square_paren, "["});
        } else if(m_stream.peek() == '=') {
            m_stream.get();
            push( { token_type::equal, "["});
        } else if(m_stream.peek() == ',') {
            m_stream.get();
            push( { token_type::comma, ","});
        } else if(std::isalpha(m_stream.peek())) {
            read_id();
        } else if(std::isdigit(m_stream.peek()) || m_stream.peek() == '-' || m_stream.peek() == '+') {
            read_number();
        } else if(m_stream.peek() == '"') {
            read_string();
        }
    }
}

void gameframework::stage_deserializer::tokenizer::ignore_characters() {
    auto is_ignorable = [](int c) {
        return c == ' ' || c == '\t' || c == '\n';
    };
    while(m_stream.peek() != EOF && is_ignorable(m_stream.peek())) {
        m_stream.get();
    }
}


void gameframework::stage_deserializer::tokenizer::read_id() {
    std::string token;
    while(std::isalpha(m_stream.peek()) || m_stream.peek() == '_') {
        token += static_cast<char>(m_stream.get());
    }
    push({gameframework::stage_deserializer::tokenizer::token_type::id, token});
}
void gameframework::stage_deserializer::tokenizer::read_number() {
    std::string token;
    while(m_stream.peek() == '-' || m_stream.peek() == '+') {
        token.append(std::to_string((char)m_stream.get()));
    }
    while(std::isdigit(m_stream.peek()) || m_stream.peek() == '.' || m_stream.peek() == 'e' || m_stream.peek() == '-') {
        token += static_cast<char>(m_stream.get());
    }
    push({gameframework::stage_deserializer::tokenizer::token_type::number, token});
}

void gameframework::stage_deserializer::tokenizer::read_string() {
    std::string token;
    assert(m_stream.get() == '"');
    while(m_stream.peek() != '"') {
        token += static_cast<char>(m_stream.get());
    }
    assert(m_stream.get() == '"');
    push({gameframework::stage_deserializer::tokenizer::token_type::string, token});
}

gameframework::stage_deserializer::tokenizer::token gameframework::stage_deserializer::tokenizer::next() {
    auto tok = m_tokens.front();
    m_tokens.pop_front();
    return tok;
}

const gameframework::stage_deserializer::tokenizer::token &gameframework::stage_deserializer::tokenizer::peek() const {
    return m_tokens.front();
}

gameframework::stage_deserializer::tokenizer::token
gameframework::stage_deserializer::tokenizer::consume(gameframework::stage_deserializer::tokenizer::token_type type) {
    auto n = next();
    assert(n.type == type);
    return n;
}
