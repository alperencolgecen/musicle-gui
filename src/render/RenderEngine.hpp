#pragma once
#include "render/SSBOManager.hpp"
#include "render/Shader.hpp"
#include <glad/glad.h>

namespace MusicLe {

class RenderEngine {
public:
  RenderEngine();
  ~RenderEngine();

  void Initialize();
  void Render(size_t particleCount, SSBOManager &ssbo);
  void Shutdown();

private:
  Shader m_ParticleShader;
  GLuint m_VAO;
};

} // namespace MusicLe
