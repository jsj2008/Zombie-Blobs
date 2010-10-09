#include "utils.hpp"

#include <cassert>
#include <cstdarg>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>
#include <errno.h>

namespace {
  void log(Log::Level l, FILE* target, const char* fmt, va_list ap) {
    static const char* prefixes[] = {"", "Error: ", "Warning: ", "Info: ", "Debug: "};
    /// @todo timestamp
    fprintf(target, prefixes[l]);
    vfprintf(target, fmt, ap);
    fprintf(target, "\n");
  }
}

namespace Utils {
  template <> std::string toString<std::string>(const std::string& t) {
    return t;
  }

  template <> std::string fromString<std::string>(const std::string& s) {
    return s;
  }

  bool fileExists(const std::string& filename) {
    struct stat buf;
    return stat(filename.c_str(), &buf) == 0;
  }

  std::string readFile(const std::string& filename) {
    std::string out;
    FILE* file = fopen(filename.c_str(), "r");
    if (!file) {
      Log::error("Failed to open '%s': %s", filename.c_str(), strerror(errno));
      return "";
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    out.reserve(size);

    char buffer[512];
    size_t pos = 0, s;
    while ((s = fread(buffer, 1, 512, file))) {
      out.insert(pos, buffer, s);
      pos += s;
    }

    if (pos != size) {
      Log::error("Unexpected EOF at %lu/%lu when reading '%s'",
                 pos, size, filename.c_str());
    }

    return out;
  }
}


Log* Log::s_log = 0;

Log::Log() : level(INFO), target(stderr) {
  if (!s_log)
    s_log = this;
}

Log::~Log() {
  if (s_log == this)
    s_log = 0;

  if (target && fileno(target) > 2) {
    fclose(target);
  }
}

#define LOG_IMPL(type) \
  assert(s_log); \
  if(s_log->level < type) return; \
  va_list ap; \
  va_start(ap, fmt); \
  log(type, s_log->target, fmt, ap); \
  va_end(ap);

void Log::error(const char* fmt, ...) { LOG_IMPL(ERROR) }
void Log::warn(const char* fmt, ...) { LOG_IMPL(WARNING) }
void Log::info(const char* fmt, ...) { LOG_IMPL(INFO) }
void Log::debug(const char* fmt, ...) { LOG_IMPL(DEBUG) }

#undef LOG_IMPL
