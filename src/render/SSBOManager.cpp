#include "SSBOManager.hpp"
#include <iostream>

namespace MusicLe {

SSBOManager::SSBOManager() : m_SSBO(0), m_Size(0) {}

SSBOManager::~SSBOManager() { Shutdown(); }

void SSBOManager::Initialize(size_t particleCount) {
  m_Size = particleCount * sizeof(ParticleData);

  glGenBuffers(1, &m_SSBO);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBO);
  glBufferData(GL_SHADER_STORAGE_BUFFER, m_Size, nullptr, GL_DYNAMIC_DRAW);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void SSBOManager::Bind(GLuint bindingPoint) {
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, m_SSBO);
}

void SSBOManager::UpdateData(const std::vector<ParticleData> &data) {
  if (data.size() * sizeof(ParticleData) > m_Size) {
    std::cerr << "SSBO Data overflow" << std::endl;
    return;
  }

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBO);
  glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0,
                  data.size() * sizeof(ParticleData), data.data());
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void SSBOManager::Shutdown() {
  if (m_SSBO) {
    glDeleteBuffers(1, &m_SSBO);
    m_SSBO = 0;
  }
}

} // namespace MusicLe
