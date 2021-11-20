//
// Created by crax on 11/20/21.
//

#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <filesystem>

namespace mikufs {
    class utils {
    public:
        static std::optional<std::string> read_file_content(const std::filesystem::path& file_path) noexcept;
        static bool write_file(const std::filesystem::path& file_path, std::string_view content) noexcept;
    };
}