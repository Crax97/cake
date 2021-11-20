//
// Created by crax on 10/29/21.
//

#include "game_framework/resources/script_resource.h"
#include "luanatic/script.h"
#include "mikufs/utils.h"
#include "logging/logger.h"

#include <filesystem>

static logging::category log_scriptresource("script resource");

void game_framework::script_resource::from_string(std::string_view str)  {
    // TODO: do content-based file reading
    auto script_source = mikufs::utils::read_file_content(str);
    if(!script_source.has_value()) {
        log_scriptresource(logging::severity::error) << "Could not read script from path " << str;
        return;
    }

    auto script = luanatic::script::compile_source(*script_source);
    if(script.has_value()) {
        m_script = std::move(*script);
        m_path = str;
    } else {
        log_scriptresource(logging::severity::error) << "Failed to compile script " << str;
    }
}

std::string game_framework::script_resource::get_script_name() const noexcept {
    if(m_script == nullptr) {
        return "<no script>";
    }

    return m_path.filename();
}
