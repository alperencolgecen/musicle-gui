#pragma once
#include <cstdio>
#include <functional>
#include <future>
#include <string>
#include <thread>

namespace MusicLe {

class PythonBridge {
public:
  static void FetchMetadata(const std::string &url,
                            std::function<void(const std::string &)> callback) {
    std::thread([url, callback]() {
      // Call Python script: python src/bridge/PythonBridge.py metadata <url>
      std::string cmd = "python src/bridge/PythonBridge.py metadata " + url;

      // Simplified popen logic
      FILE *pipe = _popen(cmd.c_str(), "r");
      if (!pipe)
        return;

      char buffer[128];
      std::string result = "";
      while (fgets(buffer, 128, pipe) != NULL) {
        result += buffer;
      }
      _pclose(pipe);

      callback(result);
    }).detach();
  }

  // Similarly for download
};

} // namespace MusicLe
