#pragma once
#include <glad/glad.h>
#include <vector>

namespace MusicLe {

struct ParticleData {
  float posX, posY;
  float prevX, prevY; // For Verlet
  float accX, accY;
  float mass;
  float padding; // Alignment
};

class SSBOManager {
public:
  SSBOManager();
  ~SSBOManager();

  void Initialize(size_t particleCount);
  void Bind(GLuint bindingPoint);
  void UpdateData(const std::vector<ParticleData> &data);
  void Shutdown();

private:
  GLuint m_SSBO;
  size_t m_Size;
};

} // namespace MusicLe
