#include "RenderEngine.hpp"
#include <iostream>

namespace MusicLe {

RenderEngine::RenderEngine() : m_VAO(0) {}

RenderEngine::~RenderEngine() { Shutdown(); }

void RenderEngine::Initialize() {
  m_ParticleShader.Load("src/shaders/particle.vert",
                        "src/shaders/particle.frag");

  glGenVertexArrays(1, &m_VAO);
  // VAO is empty because we get all data from SSBO in the vertex shader using
  // gl_InstanceID

  // Enable programmable point size (used in particle.vert)
  glEnable(GL_PROGRAM_POINT_SIZE);
}

void RenderEngine::Render(size_t particleCount, SSBOManager &ssbo) {
  m_ParticleShader.Use();

  // Set screen resolution uniform for NDC conversion in vertex shader
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  m_ParticleShader.SetVec2("screenRes", (float)viewport[2], (float)viewport[3]);

  // Bind SSBO to binding point 0 (matches physics.comp and particle.vert)
  ssbo.Bind(0);

  glBindVertexArray(m_VAO);
  // Draw particleCount points (or quads if using geometry shader/instancing)
  glDrawArraysInstanced(GL_POINTS, 0, 1, (GLsizei)particleCount);
  glBindVertexArray(0);
}

void RenderEngine::Shutdown() {
  if (m_VAO)
    glDeleteVertexArrays(1, &m_VAO);
}

} // namespace MusicLe
