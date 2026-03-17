#pragma once
#include "render/SSBOManager.hpp"
#include "render/Shader.hpp"
#include <vector>


namespace MusicLe {

class PhysicsEngine {
public:
  PhysicsEngine();
  ~PhysicsEngine();

  void Initialize(size_t particleCount, float screenWidth, float screenHeight);
  void Update(float deltaTime, float mouseX, float mouseY, float mouseStrength);
  void Shutdown();

  SSBOManager &GetSSBO() { return m_SSBO; }

private:
  SSBOManager m_SSBO;
  Shader m_ComputeShader;
  size_t m_ParticleCount;
  float m_ScreenWidth, m_ScreenHeight;
};

} // namespace MusicLe
