//
// Created by crax on 10/28/21.
//
#pragma once

#include <string>
#include <string_view>

#include "object/serializer.h"

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
    class stage_serializer : public serializer {
    private:
        std::ostream& m_stream;
        int m_tabs = 0;
    public:
        explicit stage_serializer(std::ostream& stream) : m_stream(stream) { }
        void begin() override;
        void begin_section(std::string_view section_name) override;
        void add_parameter(std::string_view param_name, std::string_view value) override;
        void end_section(std::string_view section_name) override;
        void end() override;
    };
}