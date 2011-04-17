#ifndef ZB_TR1_HPP
#define ZB_TR1_HPP

#include <cstddef>

#if defined(_MSC_VER) && ((_MSC_VER == 1500 && defined(_HAS_TR1)) || _MSC_VER > 1500)
#include <memory>
#include <functional>
#else

#if defined(__GNUC__) && __GNUC__ >= 4
#include <tr1/memory>
#include <tr1/functional>
#else
#include <boost/tr1/memory.hpp>
#include <boost/tr1/functional>
#endif

#ifndef __GXX_EXPERIMENTAL_CXX0X__
namespace std {
  using tr1::shared_ptr;
  using tr1::enable_shared_from_this;
  using tr1::bind;
  using tr1::function;
  using tr1::ref;
  namespace placeholders = tr1::placeholders;
}
#endif

#endif

#endif // ZB_TR1_HPP
