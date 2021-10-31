//
// Created by crax on 10/28/21.
//

#include "game_framework/loaders/stage_serializer.h"
#include <cassert>

#include <ostream>
#include <iomanip>

void gameframework::stage_serializer::begin() {
    m_stream << "scene [\n";
    m_tabs ++;
}

void gameframework::stage_serializer::begin_section(std::string_view section_name) {
    m_stream << std::string(m_tabs, '\t') << section_name << "[\n";
    m_tabs ++;
}

void gameframework::stage_serializer::add_parameter(std::string_view param_name, std::string_view value) {
    m_stream << std::string(m_tabs, '\t') << param_name << "=" << std::quoted(value) << "\n";
}

void gameframework::stage_serializer::end_section(std::string_view section_name) {
    m_tabs --;
    m_stream << std::string(m_tabs, '\t') << "]\n";
}

void gameframework::stage_serializer::end() {
    m_tabs --;
    m_stream << "]\n";
    m_stream.flush();
}
