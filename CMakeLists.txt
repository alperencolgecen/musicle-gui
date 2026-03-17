cmake_minimum_required(VERSION 3.20)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
project(MusicLe LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- FetchContent Dependencies ---
include(FetchContent)

# GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# Dear ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG docking
)
FetchContent_MakeAvailable(imgui)

# GLAD (OpenGL Loader — replaces GLEW for clean FetchContent builds)
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v0.1.36
)
FetchContent_MakeAvailable(glad)

# --- Source Files ---
file(GLOB SRC_FILES "src/*.cpp" "src/*/*.cpp" "src/*.c" "src/*/*.c")
file(GLOB HDR_FILES "src/*.hpp" "src/*/*.hpp" "src/*.h" "src/*/*.h")

# ImGui Core Files
set(IMGUI_SOURCES
    "${imgui_SOURCE_DIR}/imgui.cpp"
    "${imgui_SOURCE_DIR}/imgui_draw.cpp"
    "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
    "${imgui_SOURCE_DIR}/imgui_tables.cpp"
    "${imgui_SOURCE_DIR}/imgui_demo.cpp"
)

# ImGui Backend Files (GLFW + OpenGL3)
set(IMGUI_BACKEND_FILES
    "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HDR_FILES} ${IMGUI_SOURCES} ${IMGUI_BACKEND_FILES})

# --- Include Directories ---
target_include_directories(${PROJECT_NAME} PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${imgui_SOURCE_DIR}" 
    "${imgui_SOURCE_DIR}/backends"
    "${glfw_SOURCE_DIR}/include"
    "${glad_SOURCE_DIR}/include"
)

target_compile_definitions(${PROJECT_NAME} PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)

# --- Link Libraries ---
target_link_libraries(${PROJECT_NAME} PRIVATE glfw glad)

# Windows Specifics
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE opengl32)
endif()

# Note: miniaudio and sqlite3 will be included as headers in the 'include' folder.
