#include "settings.hpp"

Settings::Settings(int argc, char** argv) {
  // Defaults
  set("full screen", false);

  /// @todo load a config file

  for (char **it = argv + 1; it < argv + argc; it++) {
    std::string arg(*it);
    if (arg == "--fs") {
      set("full screen", true);
    } else if (arg == "--windowed" || arg == "--window") {
      set("full screen", false);
    }
  }
}
