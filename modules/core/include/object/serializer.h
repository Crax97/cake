#pragma once

class field;
class descriptor;

class serializer {
public:
  virtual void begin_class(const descriptor &desc, void *base) = 0;
  virtual void serialize_field(const field &desc, void *base) = 0;
  virtual void end_class(const descriptor &desc, void *base) = 0;
};