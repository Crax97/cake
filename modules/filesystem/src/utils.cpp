//
// Created by crax on 11/20/21.
//

#include "mikufs/utils.h"

#include <istream>
#include <fstream>

std::optional<std::string> mikufs::utils::read_file_content(const std::filesystem::path& file_path) noexcept {
    std::fstream input_file;
    input_file.open(file_path, std::ios::in);

    if(input_file.bad()) {
        return {};
    }

    std::string content, line;
    while(std::getline(input_file, line)) {
        content.append(line);
        content.append("\n");
    }
    return content;
}

bool mikufs::utils::write_file(const std::filesystem::path& file_path, std::string_view content) noexcept {
    std::fstream output_file;
    output_file.open(file_path, std::ios::app);

    if(output_file.bad()) {
        return false;
    }

    output_file << content;
    return true;
}