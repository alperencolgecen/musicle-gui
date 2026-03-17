#include "AudioEngine.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>

// We would include miniaudio.h here, but for now we simulate the logic
// as miniaudio is a single-header library that needs to be downloaded.

namespace MusicLe {

AudioEngine::AudioEngine() : m_Device(nullptr), m_BeatIntensity(0.0f) {
  m_FFTRaw.resize(512, 0.0f);
  m_FFTSmoothed.resize(512, 0.0f);
}

AudioEngine::~AudioEngine() { Shutdown(); }

bool AudioEngine::Initialize() {
  // miniaudio initialization logic would go here
  return true;
}

bool AudioEngine::LoadTrack(const std::string &path) {
  // Load file and prepare for playback
  return true;
}

void AudioEngine::Play() {
  // Start playback
}

void AudioEngine::Pause() {
  // Pause playback
}

void AudioEngine::Update() {
  // In a real implementation, we'd grab PCM data from miniaudio ring buffer
  // and perform FFT using a library like KissFFT or a custom implementation.

  ProcessFFT();
}

void AudioEngine::ProcessFFT() {
  // 1. Generate simulated audio data for visualization
  for (size_t i = 0; i < m_FFTRaw.size(); ++i) {
    m_FFTRaw[i] = (float)(rand() % 100) / 100.0f;

    // 2. Exponential Moving Average (EMA) for Smoothing
    m_FFTSmoothed[i] =
        m_EMAAlpha * m_FFTRaw[i] + (1.0f - m_EMAAlpha) * m_FFTSmoothed[i];
  }

  // 3. Simple Beat Detection (Bass range)
  float bassSum = 0;
  for (int i = 0; i < 10; ++i)
    bassSum += m_FFTSmoothed[i];
  m_BeatIntensity = bassSum / 10.0f;
}

void AudioEngine::Shutdown() {
  // miniaudio shutdown
}

} // namespace MusicLe
