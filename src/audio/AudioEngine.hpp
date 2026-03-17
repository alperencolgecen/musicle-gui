#ifndef __cplusplus
#error "AudioEngine.hpp must be compiled as C++"
#endif

#pragma once
#include <string>
#include <vector>

namespace MusicLe {

class AudioEngine {
public:
  AudioEngine();
  ~AudioEngine();

  bool Initialize();
  bool LoadTrack(const std::string &path);
  void Play();
  void Pause();
  void Update();
  void Shutdown();

  const std::vector<float> &GetFFTData() const { return m_FFTSmoothed; }
  float GetBeatIntensity() const { return m_BeatIntensity; }

private:
  void ProcessFFT();

  std::vector<float> m_FFTRaw;
  std::vector<float> m_FFTSmoothed;
  float m_BeatIntensity;
  float m_EMAAlpha = 0.15f; // Smoothing factor

  void *m_Device; // miniaudio device (opaque)
};

} // namespace MusicLe
