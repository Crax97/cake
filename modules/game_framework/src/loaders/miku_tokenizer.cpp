//
// Created by crax on 10/31/21.
//

#include "game_framework/loaders/miku_tokenizer.h"

#include "logging/logger.h"

void gameframework::miku_tokenizer::tokenize() {
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

void gameframework::miku_tokenizer::ignore_characters() {
    auto is_ignorable = [](int c) {
        return c == ' ' || c == '\t' || c == '\n';
    };
    while(m_stream.peek() != EOF && is_ignorable(m_stream.peek())) {
        m_stream.get();
    }
}


void gameframework::miku_tokenizer::read_id() {
    std::string token;
    while(std::isalpha(m_stream.peek()) || m_stream.peek() == '_') {
        token += static_cast<char>(m_stream.get());
    }
    push({token_type::id, token});
}
void gameframework::miku_tokenizer::read_number() {
    std::string token;
    while(m_stream.peek() == '-' || m_stream.peek() == '+') {
        token.append(std::to_string((char)m_stream.get()));
    }
    while(std::isdigit(m_stream.peek()) || m_stream.peek() == '.' || m_stream.peek() == 'e' || m_stream.peek() == '-') {
        token += static_cast<char>(m_stream.get());
    }
    push({token_type::number, token});
}

void gameframework::miku_tokenizer::read_string() {
    std::string token;
    MIKU_ASSERT(m_stream.get() == '"');
    while(m_stream.peek() != '"') {
        token += static_cast<char>(m_stream.get());
    }
    MIKU_ASSERT(m_stream.get() == '"');
    push({token_type::string, token});
}

gameframework::token gameframework::miku_tokenizer::next() {
    auto tok = m_tokens.front();
    m_tokens.pop_front();
    return tok;
}

const gameframework::token &gameframework::miku_tokenizer::peek() const {
    return m_tokens.front();
}

gameframework::token
gameframework::miku_tokenizer::consume(token_type type) {
    auto n = next();
    MIKU_ASSERT(n.type == type);
    return n;
}
