#include "object/descriptor.h"
#include "object/field.h"
#include "object/serializer.h"

#include <sstream>

class string_serializer : public serializer {
private:
  std::ostringstream output;
  int tabs = 0;

public:
  virtual void begin_class(const descriptor &desc, void *base) override {
    output << "\"" << desc.get_name() << "\": {\n";
    tabs++;
  }
  virtual void serialize_field(const field &desc, void *base) override {
    output << std::string("\t", tabs) << "\"" << desc.get_name() << "\": \""
           << desc.to_string(base) << "\",\n";
  }
  virtual void end_class(const descriptor &desc, void *base) override {
    tabs--;
    output << "}\n";
  }

  std::string get_str() const { return output.str(); }
};
