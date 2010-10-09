#include "opengl.hpp"
#include "utils.hpp"

#include <map>
#include <sstream>

void s_check_gl(const char* str, const char* file, int line) {
  static std::map<std::string, int> s_errors;
  const int limit = 10;
  int e = glGetError();
  if (e != GL_NO_ERROR) {
    std::ostringstream ss;
    ss << file << ":" << line << "\t" << (const char*)gluErrorString(e) << " (" << str << ")";
    std::string out = ss.str();
    int num = ++s_errors[out];
    if (num == limit) out += " (starting to ignore)";
    if (num <= limit) Log::error("OpenGL Error: %s", out.c_str());
  }
}
