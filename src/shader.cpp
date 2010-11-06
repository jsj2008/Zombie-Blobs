#include "shader.hpp"
#include "resource_manager.hpp"

#include <cassert>

Shader::Shader() : m_shader(0), m_type(None) {
}

Shader::~Shader() {
  if (m_shader)
    glDeleteShader(m_shader);
}

bool Shader::load(const std::string& resource, Type type) {
  m_src = ResourceManager::readFile(resource);
  if (m_src.empty()) return false;

  m_type = type;
  return true;
}

bool Shader::compile() {
  glCheck("Shader::compile");
  if (m_src.empty()) return true;

  if (!m_shader) {
    if (m_type == Vertex)
      m_shader = glRun2(glCreateShader(GL_VERTEX_SHADER));
    else if (m_type == Geometry)
      m_shader = glRun2(glCreateShader(GL_GEOMETRY_SHADER_EXT));
    else
      m_shader = glRun2(glCreateShader(GL_FRAGMENT_SHADER));
    if (!m_shader) {
      Log::error("Couldn't create shader");
      return false;
    }
  }

  const char* src = m_src.c_str();
  GLint len = m_src.length();
  glRun(glShaderSource(m_shader, 1, &src, &len));
  glRun(glCompileShader(m_shader));

  GLint ok = 0;
  glRun(glGetShaderiv(m_shader, GL_COMPILE_STATUS, &ok));
  glRun(glGetShaderiv(m_shader, GL_INFO_LOG_LENGTH, &len));
  // len may include '\0'
  if (len > 1) {
    GLchar * log = new GLchar[len];
    GLsizei size = len;
    glRun(glGetShaderInfoLog(m_shader, size, &size, log));
    const char * msg = "GLSL compiler output: %s";
    if (ok) {
      Log::info(msg, log);
    } else {
      Log::error(msg, log);
    }
    delete[] log;
  }

  if (ok)
    m_src.clear();
  return ok;
}

GLuint Shader::id() const {
  return m_shader;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

GLProgram::GLProgram() : m_prog(0), m_compiled(false) {
}

GLProgram::~GLProgram() {
  if (m_prog) glDeleteProgram(m_prog);
}

bool GLProgram::bind() {
  if (!m_compiled) {
    if (!m_prog) {
      glCheck("GLProgram::bind");
      m_prog = glRun2(glCreateProgram());
    }

    if (!m_prog)
      return false;

    for (Shaders::iterator it = m_shaders.begin(); it != m_shaders.end(); ++it) {
      if (it->compile())
        glAttachShader(m_prog, it->id());
    }

    m_compiled = true;
    /// @todo should we link only when there was a successful compiling?
    link();
  }
  if (isLinked()) {
    glRun(glUseProgram(m_prog));
    return true;
  } else
    return false;
}

void GLProgram::unbind() {
  glCheck("GLProgram::unbind");
  glRun(glUseProgram(0));
}

Shader* GLProgram::addShader(const std::string& resource, Shader::Type type) {
  m_shaders.push_back(Shader());
  Shader& shader = m_shaders.back();
  if(shader.load(resource, type)) {
    m_compiled = false;
    return &shader;
  } else {
    m_shaders.pop_back();
    return 0;
  }
}

bool GLProgram::link(bool restore) {
  glCheck("GLProgram::link");
  GLint prog = 0;
  if (restore)
    glRun(glGetIntegerv(GL_CURRENT_PROGRAM, &prog));

  glRun(glLinkProgram(m_prog));
  GLint ok = 0;
  glRun(glGetProgramiv(m_prog, GL_LINK_STATUS, &ok));

  GLint len = 0;
  glRun(glGetProgramiv(m_prog, GL_INFO_LOG_LENGTH, &len));
  // len may include '\0'
  if (len > 1) {
    GLchar * log = new GLchar[len];
    GLsizei size = len;
    glRun(glGetProgramInfoLog(m_prog, size, &size, log));
    const char * msg = "GLSL linker output: %s";
    if (ok) {
      Log::info(msg, log);
    } else {
      Log::error(msg, log);
    }
    delete[] log;
  }

  if (restore)
    glRun(glUseProgram(prog));

  return ok;
}

bool GLProgram::isLinked() {
  /// @todo use cached value
  glCheck("GLProgram::isLinked");
  if (!m_prog) return false;
  GLint b = 0;
  glRun(glGetProgramiv(m_prog, GL_LINK_STATUS, &b));
  return b;
}

GLuint GLProgram::id() const {
  return m_prog;
}
