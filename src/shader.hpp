#ifndef SHADER_HPP
#define SHADER_HPP

#include "forward.hpp"
#include "opengl.hpp"
#include "utils.hpp"

#include <list>
#include <string>

/**
 * GLSL Shader.
 *
 * It is safe to create Shader objects before actually having an OpenGL context.
 * The actual shader object is created only when the shader is compiled the first time.
 */
class Shader {
public:
  /// Supported shader types
  enum Type {
    None, Fragment, Vertex, Geometry
  };

  Shader();
  virtual ~Shader();

  /**
   * Loads shader from a resource file.
   * Returns true if loading was successful. However the shader isn't compiled
   * yet, so successful loading doesn't mean that the shader will compile/link
   * correctly.
   */
  bool load(const std::string& resource, Type type);

  /**
   * Compiles the shader if needed.
   */
  bool compile();

  /// Returns the actual OpenGL shader id, or 0 if there is no shader created yet.
  GLuint id() const;

  Type type() const { return m_type; }

protected:
  /// The actual wrapped shader object
  GLuint m_shader;

  /// The type of the shader
  Type m_type;

  std::string m_src;
};


/**
 * GLSL Program object.
 *
 * Program owns different kind of shaders (see Shader), and it handles all uniform
 * variables that the shaders use. Compiling and linking is managed from here.
 *
 * Like Shader, GLProgram objects can be safely created before any OpenGL context
 * is created; shaders are compiled, underlying program object created and the
 * program linked when the object is bind()ed first time.
 */
class GLProgram {
public:
  typedef std::list<Shader> Shaders;

  GLProgram();
  virtual ~GLProgram();

  /**
   * Binds the program object. On the first run time creates the real shader
   * program object. Also (re)compiles all the shaders and (re)links the program
   * if needed.
   */
  bool bind();

  /// Unbinds the program object.
  void unbind();

  /// Sets a uniform variable, binds the object if needed,
  /// and restores the active program before returning.
  template <typename T>
  void setUniform(const std::string& name, T t, bool restore = false) {
    glCheck("setUniform");
    GLint prog = 0;
    glRun(glGetIntegerv(GL_CURRENT_PROGRAM, &prog));
    if (!prog != m_prog)
      bind();

    GLint loc = glRun2(glGetUniformLocation(m_prog, name.c_str()));
    if(loc == -1) {
      Log::error("Failed to query uniform variable %s location", name);
    } else {
      setUniform(loc, t);
    }

    // restore the old state
    if (restore && prog != (GLint)m_prog)
      glRun(glUseProgram(prog));
  }

  void setUniform(GLint loc, GLfloat v0) { glRun(glUniform1f(loc, v0)); }
  void setUniform(GLint loc, GLfloat v0, GLfloat v1) { glRun(glUniform2f(loc, v0, v1)); }
  void setUniform(GLint loc, GLfloat v0, GLfloat v1, GLfloat v2) { glRun(glUniform3f(loc, v0, v1, v2)); }
  void setUniform(GLint loc, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) { glRun(glUniform4f(loc, v0, v1, v2, v3)); }
  void setUniform(GLint loc, GLint v0) { glRun(glUniform1i(loc, v0)); }
  void setUniform(GLint loc, GLint v0, GLint v1) { glRun(glUniform2i(loc, v0, v1)); }
  void setUniform(GLint loc, GLint v0, GLint v1, GLint v2) { glRun(glUniform3i(loc, v0, v1, v2)); }
  void setUniform(GLint loc, GLint v0, GLint v1, GLint v2, GLint v3) { glRun(glUniform4i(loc, v0, v1, v2, v3)); }

  /// Creates a new Shader and adds it to this program.
  virtual Shader* addShader(const std::string& resource, Shader::Type type);

  /**
   * (Re)Links the program.
   */
  virtual bool link(bool restore = false);

  bool isLinked();

  /// Returns the actual OpenGL program id, or 0 if there is no program created yet.
  GLuint id() const;

  Shaders& shaders() { return m_shaders; }

protected:
  GLuint m_prog;

  Shaders m_shaders;

  /// Are all shaders compiled
  bool m_compiled;
};

#endif // SHADER_HPP
