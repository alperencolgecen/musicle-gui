#include "UIEngine.hpp"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <filesystem>
#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <vector>


#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace MusicLe {

struct TrackInfo {
  const char *title;
  const char *artist;
  const char *duration;
};

static const std::vector<TrackInfo> exploreTracks = {
    {"Nebula Dreams", "Stellaris Ensemble", "4:32"},
    {"Gravitational Pull", "Nova Core", "3:45"},
    {"Cosmic Dust", "AstroWave", "5:12"},
    {"Event Horizon", "BlackHole Symphony", "6:05"}};

static const std::vector<TrackInfo> libraryTracks = {
    {"Midnight Walk", "Synthwave Kid", "3:22"},
    {"City Lights", "Neon Nights", "4:10"}};

UIEngine::UIEngine() {}

UIEngine::~UIEngine() { Shutdown(); }

void UIEngine::Initialize(GLFWwindow *window) {
  std::cout << "[UIEngine] Checking ImGui version..." << std::endl;
  
  try {
    // Create context first
    IMGUI_CHECKVERSION();
    std::cout << "[UIEngine] Creating ImGui context..." << std::endl;
    ImGui::CreateContext();
    ImGui::SetCurrentContext(ImGui::GetCurrentContext());
    
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    // Disable viewports for stability
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // --- Premium Avant-Garde Design System ---
    ImGuiStyle &style = ImGui::GetStyle();
    style.WindowRounding = 12.0f;
    style.ChildRounding = 10.0f;
    style.FrameRounding = 6.0f;
    style.GrabRounding = 6.0f;
    style.PopupRounding = 6.0f;
    style.WindowPadding = ImVec2(15, 15);
    style.FramePadding = ImVec2(10, 8);
    style.ItemSpacing = ImVec2(12, 10);
    style.WindowBorderSize = 1.0f;

    ImVec4 *colors = style.Colors;
    // Deep Night Palette
    colors[ImGuiCol_WindowBg] = ImVec4(0.02f, 0.02f, 0.03f, 0.70f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.05f, 0.05f, 0.07f, 0.40f);
    colors[ImGuiCol_Border] = ImVec4(0.20f, 0.20f, 0.25f, 0.30f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.10f, 0.15f, 0.60f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.15f, 0.15f, 0.20f, 0.80f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.20f, 0.25f, 1.00f);

    // Vibrant Accents (Cyan-Violet)
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.04f, 0.04f, 0.06f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.80f, 1.00f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.30f, 0.40f, 0.90f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.40f, 0.50f, 1.00f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.15f, 0.15f, 0.25f, 0.40f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.25f, 0.45f, 0.70f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.30f, 0.35f, 0.65f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.20f, 0.20f, 0.35f, 0.45f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.30f, 0.30f, 0.55f, 0.60f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.40f, 0.40f, 0.75f, 0.80f);
    
    std::cout << "[UIEngine] Initializing ImGui Backends..." << std::endl;
    
    // Make sure OpenGL context is current
    glfwMakeContextCurrent(window);
    
    // Initialize backends
    if (!ImGui_ImplGlfw_InitForOpenGL(window, true)) {
      std::cerr << "[UIEngine] Failed to initialize GLFW backend" << std::endl;
      throw std::runtime_error("GLFW backend initialization failed");
    }
    
    if (!ImGui_ImplOpenGL3_Init("#version 330")) {
      std::cerr << "[UIEngine] Failed to initialize OpenGL3 backend" << std::endl;
      ImGui_ImplGlfw_Shutdown();
      throw std::runtime_error("OpenGL3 backend initialization failed");
    }
    
    std::cout << "[UIEngine] Initialization complete." << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "[UIEngine] Exception during initialization: " << e.what() << std::endl;
    throw;
  } catch (...) {
    std::cerr << "[UIEngine] Unknown exception during initialization" << std::endl;
    throw;
  }
}

void UIEngine::Update() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  // Safely get viewport
  ImGuiViewport* viewport = ImGui::GetMainViewport();
  if (!viewport) {
    // Fallback to window size if viewport is not available
    int w, h;
    glfwGetWindowSize(glfwGetCurrentContext(), &w, &h);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2((float)w, (float)h));
  } else {
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
  }

  ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
      ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
      ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

  ImGui::Begin("MasterLayout", nullptr, window_flags);

  // Layout Constants
  float sidebarWidth = 280.0f;
  float playerHeight = 100.0f;
  float detailWidth = 320.0f;

  // 1. Sidebar (Navigation & Discovery)
  ImGui::SetCursorPos(ImVec2(10, 10));
  ImGui::BeginChild(
      "Sidebar",
      ImVec2(sidebarWidth - 20, viewport ? viewport->Size.y : 800 - playerHeight - 20), true);
  DrawSidebar();
  ImGui::EndChild();

  // 2. Main Content (The "Arena")
  ImGui::SetCursorPos(ImVec2(sidebarWidth, 10));
  ImGui::BeginChild("MainScene",
                    ImVec2((viewport ? viewport->Size.x : 1600) - sidebarWidth - detailWidth,
                           (viewport ? viewport->Size.y : 900) - playerHeight - 20),
                    false);
  DrawMainScene();
  ImGui::EndChild();

  // 3. Artist / Details Panel
  ImGui::SetCursorPos(ImVec2((viewport ? viewport->Size.x : 1600) - detailWidth + 10, 10));
  ImGui::BeginChild(
      "ArtistDetail",
      ImVec2(detailWidth - 20, (viewport ? viewport->Size.y : 900) - playerHeight - 20), true);
  DrawArtistDetail();
  ImGui::EndChild();

  // 4. Player Bar (Floating Glass Box)
  ImGui::SetCursorPos(ImVec2(10, (viewport ? viewport->Size.y : 900) - playerHeight + 5));
  ImGui::BeginChild("PlayerBar",
                    ImVec2((viewport ? viewport->Size.x : 1600) - 20, playerHeight - 15), true);
  DrawPlayerBar();
  ImGui::EndChild();

  DrawDownloadNotification();

  ImGui::End();
}

void UIEngine::DrawSidebar() {
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12, 12));
  ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.5f, 1.0f), "MENU");
  ImGui::Spacing();
  if (ImGui::Selectable("  Explore", m_SelectedTab == 0))
    m_SelectedTab = 0;
  if (ImGui::Selectable("  Library", m_SelectedTab == 1))
    m_SelectedTab = 1;
  if (ImGui::Selectable("  Visualizer", m_SelectedTab == 2))
    m_SelectedTab = 2;

  ImGui::Spacing();
  ImGui::Separator();
  ImGui::Spacing();

  ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.5f, 1.0f), "YOUR PLAYLISTS");
  ImGui::Spacing();
  if (ImGui::Selectable("  Night Vibes", m_SelectedTab == 3))
    m_SelectedTab = 3;
  if (ImGui::Selectable("  Coding Beats", m_SelectedTab == 4))
    m_SelectedTab = 4;
  if (ImGui::Selectable("  Starfield", m_SelectedTab == 5))
    m_SelectedTab = 5;
  ImGui::PopStyleVar();
}

void UIEngine::DrawMainScene() {
  ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.9f);

  const char *titles[] = {"EXPLORE",     "LIBRARY",      "ANTIGRAVITY",
                          "NIGHT VIBES", "CODING BEATS", "STARFIELD"};

  ImGui::SetCursorPosY(30);
  ImGui::Indent(40);

  ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.8f), "%s",
                     titles[m_SelectedTab]);
  ImGui::Separator();
  ImGui::Spacing();

  ImGui::BeginChild("TrackList",
                    ImVec2(ImGui::GetContentRegionAvail().x - 40, 0), false);

  const std::vector<TrackInfo> *tracksToDisplay = &exploreTracks;
  if (m_SelectedTab == 1 || m_SelectedTab == 3 || m_SelectedTab == 4) {
    tracksToDisplay = &libraryTracks;
  }

  if (m_SelectedTab == 2 || m_SelectedTab == 5) {
    ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.6f, 1.0f),
                       "This section visualizes audio energy...");
  } else {
    if (ImGui::BeginTable("Tracks", 4, ImGuiTableFlags_None)) {
      ImGui::TableSetupColumn("Title", ImGuiTableColumnFlags_WidthStretch,
                              2.0f);
      ImGui::TableSetupColumn("Artist", ImGuiTableColumnFlags_WidthStretch,
                              1.5f);
      ImGui::TableSetupColumn("Duration", ImGuiTableColumnFlags_WidthFixed,
                              60.0f);
      ImGui::TableSetupColumn("Action", ImGuiTableColumnFlags_WidthFixed,
                              80.0f);
      ImGui::TableHeadersRow();

      for (size_t i = 0; i < tracksToDisplay->size(); i++) {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted((*tracksToDisplay)[i].title);

        ImGui::TableSetColumnIndex(1);
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.7f, 1.0f), "%s",
                           (*tracksToDisplay)[i].artist);

        ImGui::TableSetColumnIndex(2);
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "%s",
                           (*tracksToDisplay)[i].duration);

        ImGui::TableSetColumnIndex(3);
        ImGui::PushID((int)i);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.3f, 0.2f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                              ImVec4(0.2f, 0.6f, 0.4f, 0.8f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                              ImVec4(0.3f, 0.8f, 0.5f, 1.0f));
        if (ImGui::Button("Download", ImVec2(75, 24))) {
          DownloadTrack((*tracksToDisplay)[i].title);
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();
      }
      ImGui::EndTable();
    }
  }
  ImGui::EndChild();
  // Remove the EndGroup() call as it's causing the assertion error
  // ImGui::EndGroup();

  ImGui::PopStyleVar();
}

void UIEngine::DrawArtistDetail() {
  ImGui::Text("About Artist");
  ImGui::Separator();
  ImGui::Spacing();
  // Placeholder image space
  ImGui::GetWindowDrawList()->AddRectFilled(
      ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y),
      ImVec2(ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x,
             ImGui::GetCursorScreenPos().y + 180),
      IM_COL32(40, 40, 60, 100), 8.0f);

  ImGui::Dummy(ImVec2(0, 190));
  ImGui::TextWrapped("Stellaris Ensemble explores electronic music "
                     "through synthesis and sound design...");
}

void UIEngine::DrawPlayerBar() {
  float width = ImGui::GetContentRegionAvail().x;

  // Track info
  ImGui::BeginGroup();
  ImGui::Text("Nebula Dreams");
  ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.6f, 1.0f), "Stellaris Ensemble");
  ImGui::EndGroup();

  ImGui::SameLine(width * 0.35f);

  // Controls
  ImGui::BeginGroup();
  if (ImGui::Button("Previous", ImVec2(80, 40))) {
  }
  ImGui::SameLine();
  if (ImGui::Button("Play", ImVec2(80, 40))) {
  }
  ImGui::SameLine();
  if (ImGui::Button("Next", ImVec2(80, 40))) {
  }
  ImGui::EndGroup();

  ImGui::SameLine(width * 0.75f);

  // Volume Slider
  ImGui::Text("Volume");
  ImGui::SameLine();
  static float vol = 0.5f;
  ImGui::SetNextItemWidth(-1);
  ImGui::SliderFloat("##vol", &vol, 0.0f, 1.0f, "");
}

void UIEngine::Render() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  // Viewport support disabled for stability
  /*
  if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    GLFWwindow *backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
  }
  */
}

void UIEngine::Shutdown() {
  // Check if backend is initialized before shutting down
  if (ImGui::GetCurrentContext()) {
    try {
      // Only shutdown if backends were properly initialized
      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplGlfw_Shutdown();
      ImGui::DestroyContext();
    } catch (...) {
      // Ignore any shutdown errors to prevent crashes
    }
  }
}

void UIEngine::DownloadTrack(const std::string &trackName) {
  try {
    std::filesystem::path dlPath =
        std::filesystem::current_path() / "MusicLe_Downloads";
    if (!std::filesystem::exists(dlPath)) {
      std::filesystem::create_directory(dlPath);
    }

    // Simulate a real downloaded file
    std::string safeName = trackName;
    for (char &c : safeName) {
      if (c == '<' || c == '>' || c == ':' || c == '"' || c == '/' ||
          c == '\\' || c == '|' || c == '?' || c == '*') {
        c = '_';
      }
    }

    std::filesystem::path filePath = dlPath / (safeName + ".mp3");
    std::ofstream outfile(filePath);
    outfile << "Dummy MP3 data for " << trackName;
    outfile.close();

    m_DownloadStatus = "Downloaded: " + trackName;
    m_DownloadStatusTimer = 3.0f; // Show notification for 3 seconds
  } catch (const std::exception &e) {
    m_DownloadStatus = "Failed to download!";
    m_DownloadStatusTimer = 3.0f;
  }
}

void UIEngine::DrawDownloadNotification() {
  if (m_DownloadStatusTimer > 0.0f) {
    m_DownloadStatusTimer -= ImGui::GetIO().DeltaTime;

    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(viewport->Size.x - 300, 30),
                            ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(280, 50));

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs |
        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.8f, 0.3f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);

    if (ImGui::Begin("DownloadNotification", nullptr, flags)) {
      ImGui::TextWrapped("%s", m_DownloadStatus.c_str());
    }
    ImGui::End();

    ImGui::PopStyleVar();
    ImGui::PopStyleColor(2);
  }
}

} // namespace MusicLe
