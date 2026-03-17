#include "core/App.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <exception>

int main() {
  try {
    MusicLe::App app;

    if (!app.Initialize(1600, 900, "MusicLe - Music Player")) {
      std::cerr << "Failed to initialize application" << std::endl;
      return -1;
    }

    app.Run();
    app.Shutdown();

    return 0;
  } catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return -1;
  } catch (...) {
    std::cerr << "Unknown exception occurred" << std::endl;
    return -1;
  }
}
