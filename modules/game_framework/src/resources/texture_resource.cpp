//
// Created by crax on 10/29/21.
//

#include "game_framework/resources/texture_resource.h"

#include "game_framework/loaders/texture_loader.h"

void game_framework::texture_resource::from_string(std::string_view str)  {
    auto texture_loaded = gameframework::texture_loader::load_texture(str);
    if(texture_loaded) {
        m_texture = std::move(texture_loaded.value());
    }
    m_path = str;
}