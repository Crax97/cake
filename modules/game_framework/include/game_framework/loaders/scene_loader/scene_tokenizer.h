//
// Created by crax on 10/28/21.
//
#pragma once

#include <string>
#include <istream>
#include <list>

namespace game_framework::scene_loader {
     enum class type {
         scene,
         word,
         string,
         boolean,
         number,
         left_square_bracket,
         right_square_bracket,
         equals
     };
     class scene_token {
     private:
         type m_type;
         std::string m_spelling;
     public:
         scene_token(type type, const std::string& spelling) : m_type(type), m_spelling(spelling) {}

         [[nodiscard]] type get_type() const { return m_type; }

         [[nodiscard]] std::string_view get_spelling() const {
             return m_spelling;
         };

         bool operator==(const scene_token &other) const {
             return m_type == other.m_type && m_spelling == other.m_spelling;
         }
     };
     class tokenizer {
     private:
         std::istream& m_stream;
         std::list<scene_token> m_token_stack;

         scene_token parse_number();
         scene_token parse_string();
         scene_token parse_word();
     public:
        explicit tokenizer(std::istream& stream) : m_stream(stream) {}
        void tokenize();
        [[nodiscard]] const scene_token& peek() const;

        scene_token pop();
        scene_token consume(std::initializer_list<type> expected_token_types);
     };
 }