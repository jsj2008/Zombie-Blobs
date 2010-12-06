#ifndef OPENGL_HPP
#define OPENGL_HPP

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif

#include "GL/glew.h"
#include <GL/gl.h>

void s_check_gl(const char* str, const char* file, int line);

template <typename T>
T s_check_gl2(const char* str, const T& t, const char* file, int line) {
  s_check_gl(str, file, line);
  return t;
}

#define glCheck(str) s_check_gl(str, __FILE__, __LINE__)
#define glRun(cmd) do { (void)cmd; glCheck(#cmd); } while (false)
#define glRun2(cmd) s_check_gl2(#cmd, cmd, __FILE__, __LINE__)

#endif // OPENGL_HPP
