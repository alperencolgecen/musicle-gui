# MusicLe

MusicLe is a C++ OpenGL desktop application built with CMake, GLFW, Dear ImGui, and GLAD.

## What this repo provides

- Source code under `src/` and `include/`
- CMake build support for Windows + Visual Studio
- Automatic dependency download for `glfw`, `imgui`, and `glad`
- A ready-to-build structure for new contributors

## Requirements

- Windows 10 / 11
- Visual Studio 2022 with **Desktop development with C++**
- CMake 3.20 or newer
- Python 3.x available on PATH (required by GLAD generator)

## Quick start

From the repository root:

```powershell
git clone <repo-url>
cd MusicLe
mkdir build_vs2022
cd build_vs2022
"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Debug
```

If `cmake` is already on your PATH, you may use this shorter form:

```powershell
cmake -S . -B build_vs2022 -G "Visual Studio 17 2022" -A x64
cmake --build build_vs2022 --config Debug
```

## Run the application

After a successful build, run:

```powershell
cd build_vs2022\Debug
.\MusicLe.exe
```

## Notes

- The repo ignores generated build folders like `build/` and `build_vs2022/`.
- `MusicLe_Downloads/` is also ignored because it contains local downloads only.
- `imgui.ini` and `tmp_test.cpp` are not tracked by default.

## Dependencies

The project automatically downloads and builds:

- `glfw` for window and input management
- `imgui` for the graphical user interface
- `glad` for OpenGL function loading

On Windows, the project also links to `opengl32`.

## Common issues

- If Visual Studio reports missing toolset `v145`, create a fresh build folder and use the current Visual Studio generator:

```powershell
cmake -S . -B build_vs2022 -G "Visual Studio 17 2022" -A x64
```

- If GLAD fails, make sure Python 3 is installed and available on PATH.

## Contributing

If you want to help, please open an issue or submit a pull request. Keep changes focused, and add documentation for any new features.
