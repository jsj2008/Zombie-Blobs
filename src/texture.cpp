#include "texture.hpp"
#include "opengl.hpp"
#include "utils.hpp"

#include <cassert>

Texture::Texture()
  : m_internalFormat(4), m_id(0), m_boundTexture(0),
  m_format(GL_RGBA), m_dataType(GL_UNSIGNED_BYTE)
{
  m_parameters[GL_TEXTURE_WRAP_S] = GL_CLAMP_TO_EDGE;
  m_parameters[GL_TEXTURE_WRAP_T] = GL_CLAMP_TO_EDGE;
  m_parameters[GL_TEXTURE_MAG_FILTER] = GL_LINEAR;
  m_parameters[GL_TEXTURE_MIN_FILTER] = GL_LINEAR;
}

Texture::~Texture() {
  if (m_id)
    glDeleteTextures(1, &m_id);
}

void Texture::bind(int texture, int width, int height) {
  if (m_id == 0) setup(0, width, height);
  m_boundTexture = texture;
  glRun(glActiveTexture(GL_TEXTURE0 + m_boundTexture));
  glRun(glBindTexture(GL_TEXTURE_2D, m_id));
}

void Texture::bind(int texture) {
  assert(m_id);
  m_boundTexture = texture;
  glRun(glActiveTexture(GL_TEXTURE0 + m_boundTexture));
  glRun(glBindTexture(GL_TEXTURE_2D, m_id));
}

void Texture::unbind() {
  glRun(glActiveTexture(GL_TEXTURE0 + m_boundTexture));
  glRun(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::setup(unsigned int fbo, int width, int height) {
  if (m_id == 0) glRun(glGenTextures(1, &m_id));

  bool type_changed = m_type != m_active_type,
       size_changed = m_width != width || m_height != height,
       fbo_changed = m_fbos.find(fbo) == m_fbos.end();

  if (type_changed || size_changed) {
    bind();
    texImage(width, height);

    for (Parameters::iterator it = m_parameters.begin(); it != m_parameters.end(); ++it) {
      glRun(glTexParameteri(GL_TEXTURE_2D, it->first, it->second));
    }

    unbind();
  }

  /// @todo Should we run this if only size was changed?
  if (fbo && (type_changed || size_changed || fbo_changed))
    glRun(glFramebufferTexture2D(GL_FRAMEBUFFER, m_type, GL_TEXTURE_2D, m_id, 0 /* level */));

  m_width = width;
  m_height = height;
  m_active_type = m_type;
  if (fbo_changed) m_fbos.insert(fbo);
}

void Texture::texImage(int width, int height) {
  if (m_type == GL_DEPTH_ATTACHMENT) {
    glRun(glTexImage2D(GL_TEXTURE_2D, 0 /* level */, GL_DEPTH_COMPONENT24, width, height,
                       0 /* border */, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL /* data */));
  } else if (m_type == GL_STENCIL_ATTACHMENT) {
    /// @todo implement
  } else {
    glRun(glTexImage2D(GL_TEXTURE_2D, 0 /* level */, m_internalFormat, width, height,
                       0 /* border */, m_format, m_dataType, getData() /* data */));
  }
}
