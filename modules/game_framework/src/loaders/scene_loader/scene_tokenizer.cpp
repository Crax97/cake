//
// Created by crax on 10/28/21.
//

#include "game_framework/loaders/scene_loader/scene_tokenizer.h"

#include <cassert>

void game_framework::scene_loader::tokenizer::tokenize() {

}

const game_framework::scene_loader::scene_token &game_framework::scene_loader::tokenizer::peek() const {
    return m_token_stack.back();
}

game_framework::scene_loader::scene_token game_framework::scene_loader::tokenizer::pop() {
    auto token = m_token_stack.back();
    m_token_stack.pop_back();
    return token;
}

game_framework::scene_loader::scene_token
game_framework::scene_loader::tokenizer::consume(std::initializer_list<type> expected_token_types) {
    for(auto& ttype : expected_token_types) {
        if(peek().get_type() == ttype) {
            return pop();
        }
    }

    assert(false && "Did not find the expected token!");
}

game_framework::scene_loader::scene_token game_framework::scene_loader::tokenizer::parse_number() {
}

game_framework::scene_loader::scene_token game_framework::scene_loader::tokenizer::parse_string() {
}

game_framework::scene_loader::scene_token game_framework::scene_loader::tokenizer::parse_word() {
}
