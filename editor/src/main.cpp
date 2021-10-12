#include "editor_application.h"

int main(int argc, char **argv) {
  editor_application app(argc, argv);
  return app.loop();
}