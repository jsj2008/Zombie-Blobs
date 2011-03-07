#include "utils.hpp"

#include <cassert>
#include <cstdarg>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>
#include <errno.h>

#ifndef _WIN32
#include <sys/time.h>
#endif

namespace {
  void log(Log::Level l, FILE* target, const char* fmt, va_list ap) {
    static const char* prefixes[] = {"", "Error:", "Warning:", "Info:", "Debug:"};
#ifdef _WIN32
    // this is untested, if this compiles then remove this comment :p
    SYSTEMTIME lt;
    GetLocalTime(&lt);
    fprintf(target, "%02d:%02d:%02d.%03ld ", lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds);
#else
    struct timeval tv;
    gettimeofday(&tv, 0);
    struct tm* tm = localtime(&tv.tv_sec);
    fprintf(target, "%02d:%02d:%02d.%03ld ", tm->tm_hour, tm->tm_min, tm->tm_sec, tv.tv_usec/1000);
#endif

    fprintf(target, "%8s ", prefixes[l]);
    vfprintf(target, fmt, ap);
    fputc('\n', target);
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

Log::Log() : level(LOG_INFO), target(stderr) {
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

void Log::error(const char* fmt, ...) { LOG_IMPL(LOG_ERROR) }
void Log::warn(const char* fmt, ...) { LOG_IMPL(LOG_WARNING) }
void Log::info(const char* fmt, ...) { LOG_IMPL(LOG_INFO) }
void Log::debug(const char* fmt, ...) { LOG_IMPL(LOG_DEBUG) }

#undef LOG_IMPL
