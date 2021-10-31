//
// Created by crax on 10/31/21.
//
#pragma once

#include <istream>
#include <string>
#include <list>

namespace gameframework {
    enum class token_type {
        id,
        number,
        string,
        equal,
        comma,
        left_square_paren,
        right_square_paren,
    };
    struct token {
        token_type type;
        std::string spelling;
    };
    class miku_tokenizer {
    private:
        std::istream &m_stream;
    public:


    private:
        std::list <token> m_tokens;

        void push(const token &tok) {
            m_tokens.push_back(tok);
        }

        void read_id();

        void read_number();

        void read_string();

        void ignore_characters();

    public:
        explicit miku_tokenizer(std::istream &stream)
                : m_stream(stream) {}

        void tokenize();

        token next();

        [[nodiscard]] const token &peek() const;

        token consume(token_type type);
    };
}