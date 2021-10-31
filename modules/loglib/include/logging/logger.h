//
// Created by crax on 10/31/21.
//

#pragma once

#include <string>
#include <sstream>
#include <utility>

namespace logging {
    enum class severity {
        info,
        warning,
        error,
        fatal // This one kills the application!
    };

    class category {
    private:
        std::string m_category_name;

        class message {
        private:
            std::stringstream m_stream;
            severity m_severity;
        public:
            explicit message(severity sev, std::string_view category_name);

            template<typename T>
            message& operator<<(const T& el) {
                m_stream << el;
                return *this;
            }

            ~message();
        };

    public:
        explicit category(std::string category_name)
            : m_category_name{std::move(category_name)} { }

        message operator()(severity sev = severity::info);
    };
}