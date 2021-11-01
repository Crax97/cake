//
// Created by crax on 10/31/21.
//

#include "logging/logger.h"

#include <iostream>
#include <csignal>

std::string_view severity_to_escape_sequence(logging::severity sev) {
    switch(sev) {
        case logging::severity::info:
            return "\u001b[32m";
        case logging::severity::warning:
            return "\u001b[33m";
        case logging::severity::error:
            return "\u001b[35m";
        case logging::severity::fatal:
            return "\u001b[31m";
    }
    return "\u001b[32m";
}

logging::category::message logging::category::operator()(logging::severity sev) {
    return message(sev, m_category_name);
}

logging::category::message::message(logging::severity sev, std::string_view category_name)
    : m_severity(sev) {
    m_stream << severity_to_escape_sequence(sev);
    if(sev == severity::fatal) {
        m_stream << "-------------------FATAL ERROR--------------------\n";
    }
    m_stream << "[" << category_name << "] ";
}

logging::category::message::~message() {
    m_stream << "\u001b[0m\n";
    if(m_severity >= severity::error) {
        std::cerr << m_stream.str();
    } else {
        std::cout << m_stream.str();
    }
    if(m_severity == severity::fatal) {
        std::raise(SIGTRAP);
        std::exit(-1);
    }
}
