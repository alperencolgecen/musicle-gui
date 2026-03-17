#pragma once
#include "imgui.h"
#include <string>

struct GLFWwindow;

namespace MusicLe {

class UIEngine {
public:
  UIEngine();
  ~UIEngine();

  void Initialize(GLFWwindow *window);
  void Update();
  void Render();
  void Shutdown();

private:
  void DrawSidebar();
  void DrawMainScene();
  void DrawArtistDetail();
  void DrawPlayerBar();

  // UI state
  float m_SidebarWidth = 250.0f;
  float m_DetailWidth = 300.0f;
  float m_PlayerBarHeight = 90.0f;

  int m_SelectedTab = 0;

  // Downloading state
  std::string m_DownloadStatus;
  float m_DownloadStatusTimer = 0.0f;

  void DownloadTrack(const std::string &trackName);
  void DrawDownloadNotification();
};

} // namespace MusicLe
