#pragma once
#include <glad/glad.h>
#include <string>

namespace MusicLe {

class Shader {
public:
  Shader();
  ~Shader();

  bool Load(const std::string &vertexPath, const std::string &fragmentPath);
  bool LoadCompute(const std::string &computePath);
  void Use();

  void SetInt(const std::string &name, int value);
  void SetFloat(const std::string &name, float value);
  void SetVec2(const std::string &name, float x, float y);

  GLuint GetID() const { return m_ID; }

private:
  std::string ReadFile(const std::string &path);
  void CheckCompileErrors(GLuint shader, std::string type);

  GLuint m_ID;
};

} // namespace MusicLe
