#include <chrono>
#include <iostream>
#include <thread>


#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "../audio/AudioEngine.hpp"
#include "../physics/PhysicsEngine.hpp"
#include "../render/RenderEngine.hpp"
#include "../ui/UIEngine.hpp"
#include "App.hpp"


namespace MusicLe {

class AppInternal {
public:
  PhysicsEngine physics;
  RenderEngine renderer;
  AudioEngine audio;
  UIEngine ui;
};

App::App() : m_Window(nullptr), m_Running(false) {
  m_Internal = new AppInternal();
}

App::~App() {
  Shutdown();
  delete m_Internal;
}

bool App::Initialize(int width, int height, const std::string &title) {
  if (!glfwInit())
    return false;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  std::cout << "[App] Creating window..." << std::endl;
  m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if (!m_Window) {
    std::cerr << "[App] Failed to create window!" << std::endl;
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(m_Window);
  glfwSwapInterval(0);

  std::cout << "[App] Loading GLAD..." << std::endl;
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "[App] Failed to initialize GLAD" << std::endl;
    return false;
  }

  // Check OpenGL version
  std::cout << "[App] OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
  const char* version = (const char*)glGetString(GL_VERSION);
  int major = 0, minor = 0;
  if (sscanf(version, "%d.%d", &major, &minor) == 2) {
    if (major < 4 || (major == 4 && minor < 3)) {
      std::cout << "[App] Warning: OpenGL 4.3 not available, disabling compute shaders" << std::endl;
    }
  }

  std::cout << "[App] Initializing subsystems..." << std::endl;
  m_Internal->physics.Initialize(50000, (float)width, (float)height);
  m_Internal->renderer.Initialize();
  m_Internal->audio.Initialize();

  std::cout << "[App] Initializing UI..." << std::endl;
  m_Internal->ui.Initialize(m_Window);

  std::cout << "[App] Initialization complete." << std::endl;
  m_Running = true;
  return true;
}

void App::Run() {
  auto lastFrameTime = std::chrono::high_resolution_clock::now();

  while (m_Running && !glfwWindowShouldClose(m_Window)) {
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = currentTime - lastFrameTime;
    double deltaTime = elapsed.count();

    if (deltaTime >= m_FrameTime) {
      HandleEvents();
      Update(deltaTime);
      Render();

      lastFrameTime = currentTime;
    } else {
      std::this_thread::sleep_for(std::chrono::microseconds(500));
    }
  }
}

void App::HandleEvents() { glfwPollEvents(); }

void App::Update(double deltaTime) {
  double mouseX, mouseY;
  glfwGetCursorPos(m_Window, &mouseX, &mouseY);

  // Mouse interaction strength
  float strength =
      glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS
          ? 5000.0f
          : -2000.0f;

  m_Internal->audio.Update();
  float beat = m_Internal->audio.GetBeatIntensity();

  // Audio-visual mapping: Beat affects mouse interaction strength
  float finalStrength = strength * (1.0f + beat * 5.0f);

  /*
  m_Internal->physics.Update((float)deltaTime, (float)mouseX, (float)mouseY,
                             finalStrength);
  */

  m_Internal->ui.Update();
}

void App::Render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.02f, 0.02f, 0.03f, 1.0f);

  int w, h;
  glfwGetWindowSize(m_Window, &w, &h);
  glViewport(0, 0, w, h);

  // m_Internal->renderer.Render(50000, m_Internal->physics.GetSSBO());
  m_Internal->ui.Render();

  glfwSwapBuffers(m_Window);
}

void App::Shutdown() {
  if (m_Window) {
    m_Internal->ui.Shutdown();
    m_Internal->renderer.Shutdown();
    m_Internal->physics.Shutdown();
    m_Internal->audio.Shutdown();
    
    glfwDestroyWindow(m_Window);
    glfwTerminate();
    m_Window = nullptr;
  }
}

} // namespace MusicLe
