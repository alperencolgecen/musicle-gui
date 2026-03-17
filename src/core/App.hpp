#pragma once
#include <GLFW/glfw3.h>
#include <string>

namespace MusicLe {

class App {
public:
  App();
  ~App();

  bool Initialize(int width, int height, const std::string &title);
  void Run();
  void Shutdown();

private:
  void Update(double deltaTime);
  void Render();
  void HandleEvents();

  GLFWwindow *m_Window;
  bool m_Running;
  class AppInternal *m_Internal;

  // Timing for 144 FPS
  double m_TargetFPS = 144.0;
  double m_FrameTime = 1.0 / m_TargetFPS;
};

} // namespace MusicLe
