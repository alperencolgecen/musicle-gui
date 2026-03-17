#include "Shader.hpp"
#include <fstream>
#include <iostream>
#include <sstream>


namespace MusicLe {

Shader::Shader() : m_ID(0) {}

Shader::~Shader() {
  if (m_ID)
    glDeleteProgram(m_ID);
}

std::string Shader::ReadFile(const std::string &path) {
  std::ifstream file(path);
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

void Shader::CheckCompileErrors(GLuint shader, std::string type) {
  GLint success;
  GLchar infoLog[1024];
  if (type != "PROGRAM") {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                << infoLog << std::endl;
      // Instead of abort, just print error and continue
      return;
    }
  } else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                << infoLog << std::endl;
      // Instead of abort, just print error and continue
      return;
    }
  }
}

bool Shader::Load(const std::string &vertexPath,
                  const std::string &fragmentPath) {
  std::string vertexCode = ReadFile(vertexPath);
  std::string fragmentCode = ReadFile(fragmentPath);
  
  if (vertexCode.empty() || fragmentCode.empty()) {
    std::cerr << "ERROR::SHADER_FILE_NOT_FOUND: " << vertexPath << " or " << fragmentPath << std::endl;
    return false;
  }
  
  const char *vShaderCode = vertexCode.c_str();
  const char *fShaderCode = fragmentCode.c_str();

  GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);
  CheckCompileErrors(vertex, "VERTEX");

  GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);
  CheckCompileErrors(fragment, "FRAGMENT");

  m_ID = glCreateProgram();
  glAttachShader(m_ID, vertex);
  glAttachShader(m_ID, fragment);
  glLinkProgram(m_ID);
  CheckCompileErrors(m_ID, "PROGRAM");

  glDeleteShader(vertex);
  glDeleteShader(fragment);
  return true;
}

bool Shader::LoadCompute(const std::string &computePath) {
  std::string computeCode = ReadFile(computePath);
  
  if (computeCode.empty()) {
    std::cerr << "ERROR::COMPUTE_SHADER_FILE_NOT_FOUND: " << computePath << std::endl;
    return false;
  }
  
  const char *cShaderCode = computeCode.c_str();

  GLuint compute = glCreateShader(GL_COMPUTE_SHADER);
  glShaderSource(compute, 1, &cShaderCode, NULL);
  glCompileShader(compute);
  CheckCompileErrors(compute, "COMPUTE");

  m_ID = glCreateProgram();
  glAttachShader(m_ID, compute);
  glLinkProgram(m_ID);
  CheckCompileErrors(m_ID, "PROGRAM");

  glDeleteShader(compute);
  return true;
}

void Shader::Use() { glUseProgram(m_ID); }

void Shader::SetInt(const std::string &name, int value) {
  glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string &name, float value) {
  glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::SetVec2(const std::string &name, float x, float y) {
  glUniform2f(glGetUniformLocation(m_ID, name.c_str()), x, y);
}

} // namespace MusicLe
