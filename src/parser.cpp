#include "parser.hpp"
#include "utils.hpp"

#include <fstream>
#include <sstream>
#include <set>

Parser::Parser() : m_lineno(0) {}

void Parser::process(std::istream& is) {
  std::string line, prefix;

  std::set<std::string> errors;

  while (!(std::getline(is, line).rdstate() & std::ios_base::failbit)) {
    ++m_lineno;
    if (line.empty() || line[0] == '#') continue;

    std::istringstream ss(line);
    ss.exceptions(std::ios_base::failbit | std::ios_base::badbit);

    ss >> prefix;

    std::map<std::string, Handler>::const_iterator it = m_handlers.find(prefix);
    if (it != m_handlers.end()) {
      it->second(ss);
      if (ss.rdstate() & std::ios_base::failbit) {
        Log::error("Failed to parse %s:%d: %s", m_file.c_str(), m_lineno, line.c_str());
      }
      if (!(ss.rdstate() & std::ios_base::eofbit)) {
        std::string tmp;
        ss.exceptions(std::ios_base::badbit);
        ss >> tmp;
        if (!(ss.rdstate() & std::ios_base::eofbit)) {
          Log::error("Failed to parse %s:%d (extra data): %s", m_file.c_str(), m_lineno, line.c_str());
        }
      }
    } else if (errors.find(prefix) == errors.end()) {
      errors.insert(prefix);
      Log::error("Unknown prefix '%s' at %s:%d", prefix.c_str(), m_file.c_str(), m_lineno);
    }
  }
}

bool Parser::load(const std::string & filename) {
  m_lineno = 0;
  m_file = filename;
  std::ifstream is;
  is.open(filename.c_str());
  if (is.rdstate() & std::ios_base::failbit) {
    Log::error("Failed to load %s", filename.c_str());
  } else {
    try {
      is.exceptions(std::ios_base::badbit);
      start();
      process(is);
      finish();
      return true;
    } catch (std::ios_base::failure& err) {
      Log::error("Parser error at %s:%d: %s", m_file.c_str(), m_lineno, err.what());
    }
  }
  return false;
}
