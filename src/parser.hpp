#ifndef PARSER_HPP
#define PARSER_HPP

#include "zb_tr1.hpp"

#include <LinearMath/btAlignedObjectArray.h>

#include <map>
#include <string>
#include <istream>

namespace {
  template <typename T> void setter(T& t, std::istream& is) { is >> t; }
  template <typename T> void inserter(btAlignedObjectArray<T>& t, std::istream& is) { T v; is >> v; t.push_back(v); }
}

class Parser
{
public:
  Parser();
  bool load(const std::string& filename);

protected:
  void process(std::istream& is);
  virtual void start() {}
  virtual void finish() {}

  typedef std::function<void (std::istream&)> Handler;
  std::map<std::string, Handler> m_handlers;

  template <typename T>
  void reg(std::string prefix, T& t) {
    using namespace std::placeholders;
    m_handlers[prefix] = std::bind(&setter<T>, std::ref(t), _1);
  }
  template <typename T>
  void reg(std::string prefix, void (T::*func)(std::istream&)) {
    using namespace std::placeholders;
    m_handlers[prefix] = std::bind(func, static_cast<T*>(this), _1);
  }
  template <typename T>
  void reg(std::string prefix, btAlignedObjectArray<T>& t) {
    using namespace std::placeholders;
    m_handlers[prefix] = std::bind(&inserter<T>, std::ref(t), _1);
  }
  void reg(std::string prefix, Handler func) {
    m_handlers[prefix] = func;
  }

  int m_lineno;
  std::string m_file;
};

#endif
