#include "PhysicsEngine.hpp"
#include <glad/glad.h>
#include <random>
#include <cstdio>
#include <iostream>

namespace MusicLe {

PhysicsEngine::PhysicsEngine()
    : m_ParticleCount(0), m_ScreenWidth(0), m_ScreenHeight(0) {}

PhysicsEngine::~PhysicsEngine() { Shutdown(); }

void PhysicsEngine::Initialize(size_t particleCount, float screenWidth,
                               float screenHeight) {
  m_ParticleCount = particleCount;
  m_ScreenWidth = screenWidth;
  m_ScreenHeight = screenHeight;

  m_SSBO.Initialize(m_ParticleCount);
  
  // Check OpenGL version before loading compute shader
  const char* version = (const char*)glGetString(GL_VERSION);
  int major = 0, minor = 0;
  if (sscanf(version, "%d.%d", &major, &minor) == 2) {
    if (major >= 4 && (major > 4 || minor >= 3)) {
      std::cout << "[PhysicsEngine] Loading compute shader..." << std::endl;
      if (!m_ComputeShader.LoadCompute("src/shaders/physics.comp")) {
        std::cout << "[PhysicsEngine] Failed to load compute shader, using CPU fallback" << std::endl;
      }
    } else {
      std::cout << "[PhysicsEngine] Compute shaders not supported, using CPU fallback" << std::endl;
    }
  }

  // Initialize particles with random positions
  std::vector<ParticleData> initialData(m_ParticleCount);
  std::mt19937 rng(1337);
  std::uniform_real_distribution<float> distWidth(0, screenWidth);
  std::uniform_real_distribution<float> distHeight(0, screenHeight);
  std::uniform_real_distribution<float> distMass(0.5f, 2.0f);

  for (auto &p : initialData) {
    float x = distWidth(rng);
    float y = distHeight(rng);
    p.posX = x;
    p.posY = y;
    p.prevX = x;
    p.prevY = y;
    p.accX = 0;
    p.accY = 0;
    p.mass = distMass(rng);
  }

  m_SSBO.UpdateData(initialData);
}

void PhysicsEngine::Update(float deltaTime, float mouseX, float mouseY,
                           float mouseStrength) {
  // Only use compute shader if it was successfully loaded
  GLuint shaderID = m_ComputeShader.GetID();
  if (shaderID != 0) {
    m_ComputeShader.Use();
    m_ComputeShader.SetFloat("deltaTime", deltaTime);
    m_ComputeShader.SetVec2("mousePos", mouseX, mouseY);
    m_ComputeShader.SetFloat("mouseStrength", mouseStrength);
    m_ComputeShader.SetVec2("screenRes", m_ScreenWidth, m_ScreenHeight);

    m_SSBO.Bind(0);

    // Dispatch compute shader
    uint32_t numGroups = (uint32_t)((m_ParticleCount + 255) / 256);
    glDispatchCompute(numGroups, 1, 1);

    // Memory barrier to ensure compute is done before rendering
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
  } else {
    // CPU fallback - do nothing for now (particles stay static)
    // In a real implementation, you would do physics calculations on CPU here
  }
}

void PhysicsEngine::Shutdown() { m_SSBO.Shutdown(); }

} // namespace MusicLe
