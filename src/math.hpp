#ifndef MATH_HPP
#define MATH_HPP

#include <cmath>

#ifndef M_PI
# define M_PI 3.141592653589793238462643
#endif
#ifndef M_PI_2
# define M_PI_2 1.57079632679489661923
#endif

#ifdef _WIN32
namespace std {
  template <typename T> inline int isfinite(T f) { return _finite(f); }
}
#endif

#endif // MATH_HPP
