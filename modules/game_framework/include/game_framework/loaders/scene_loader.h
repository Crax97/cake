//
// Created by crax on 10/28/21.
//
#pragma once

#include <string>
#include <string_view>

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
    class scene_loader {
    private:
    public:
    };
}