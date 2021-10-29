#pragma once

#include <string_view>

class field;
class descriptor;

class serializer {
public:
    virtual void begin() = 0;
    virtual void begin_section(std::string_view section_name) = 0;
    virtual void add_parameter(std::string_view param_name, std::string_view value) = 0;
    virtual void end_section(std::string_view section_name) = 0;
    virtual void end() = 0;
};