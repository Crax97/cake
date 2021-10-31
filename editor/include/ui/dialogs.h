//
// Created by crax on 10/30/21.
//

#pragma once

#include <optional>
#include <filesystem>

namespace editor::dialogs {
    std::optional<std::filesystem::path> file_picker(std::string_view window_title, std::initializer_list<std::string_view> extensions,
                                                     const std::filesystem::path& root, std::string_view file_description,
                                                     bool allow_multiple_select);
    std::optional<std::filesystem::path> save_file(std::string_view window_title, std::initializer_list<std::string_view> extensions, const std::filesystem::path& root,
                                                   std::string_view file_description);
    std::optional<std::string> text_input(std::string_view window_title, std::string_view message, std::string_view default_input);
}