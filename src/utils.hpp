#ifndef UTILS_HPP
#define UTILS_HPP

#include <stdio.h>
#include <sstream>

namespace Utils {
  template <typename T>
  std::string toString(const T& t) {
    std::ostringstream ss;
    ss << t;
    return ss.str();
  }

  template <> std::string toString<std::string>(const std::string& t);

  template <typename T>
  T fromString(const std::string& s) {
    std::istringstream ss(s);
    T t;
    ss >> t;
    return t;
  }

  template <> std::string fromString<std::string>(const std::string& s);

  bool fileExists(const std::string& filename);
}


#ifdef __GNUC__
#define PRINTF_CHECK __attribute__ ((format (printf, 1, 2)))
#else
#define PRINTF_CHECK
#endif

class Log {
public:
  enum Level { NONE = 0, ERROR, WARNING, INFO, DEBUG } level;
  FILE* target;

  Log();
  ~Log();

  static void error(const char* fmt, ...) PRINTF_CHECK;
  static void warn(const char* fmt, ...) PRINTF_CHECK;
  static void info(const char* fmt, ...) PRINTF_CHECK;
  static void debug(const char* fmt, ...) PRINTF_CHECK;

private:
  static Log* s_log;
};

#undef PRINTF_CHECK

#endif // UTILS_HPP
