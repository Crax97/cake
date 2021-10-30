//
// Created by crax on 10/30/21.
//

/*
 * This is just a wrapper around the utilities provided by tinyfiledialogs
 * */

#include "ui/dialogs.h"
#include "tinyfiledialogs.h"

#include <string_view>

const char** to_c_str_extensions(std::initializer_list<std::string_view> extensions) {
    if(extensions.size() == 0) {
        return nullptr;
    }
    const char** c_extensions = new const char*[extensions.size()];
    int i = 0;
    for(auto& extension : extensions) {
    c_extensions[i] = extension.data();
    i++;
    }
    return c_extensions;
}

std::optional<std::filesystem::path>
editor::dialogs::file_picker(std::string_view window_title, std::initializer_list<std::string_view> extensions, const std::filesystem::path& root,
                             std::string_view file_description, bool allow_multiple_select) {
    const char** c_extensions = to_c_str_extensions(extensions);
    char* c_path = tinyfd_openFileDialog(window_title.data(), root.c_str(), extensions.size(), c_extensions, file_description.data(), allow_multiple_select);
    if(c_path == nullptr) {
        return {};
    }
    std::filesystem::path path = c_path;
    delete[] c_extensions;
    return path;
}

std::optional<std::filesystem::path>
editor::dialogs::save_file(std::string_view window_title, std::initializer_list<std::string_view> extensions, const std::filesystem::path& root,
                           std::string_view file_description) {
    const char** c_extensions = to_c_str_extensions(extensions);
    char* c_path = tinyfd_saveFileDialog(window_title.data(), root.c_str(), extensions.size(), c_extensions, file_description.data());
    if(c_path == nullptr) {
        return {};
    }
    std::filesystem::path path = c_path;
    delete[] c_extensions;
    return path;
}
