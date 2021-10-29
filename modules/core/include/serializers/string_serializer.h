#include "object/descriptor.h"
#include "object/field.h"
#include "object/serializer.h"

#include <sstream>

class string_serializer : public serializer {
private:
  std::ostringstream output;
  int tabs = 0;

public:

    void begin() override {
        output << "{\n";
        tabs ++;
    }
    void begin_section(std::string_view section_name) override {
        output << std::string(tabs, '\t') << '"' << section_name << '"' << " : {\n";
        tabs++;
    }
    void add_parameter(std::string_view param_name, std::string_view value) override {
        output << std::string(tabs, '\t') << '"' << param_name << '"' << " : " << value << ",\n";
    }
    void end_section(std::string_view section_name) override {
        tabs --;
        output << std::string(tabs, '\t') << "},\n";
    }
    void end() override {
        tabs --;
        output << "}";
    }
    std::string get_str() const { return output.str(); }
};
