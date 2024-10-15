#ifndef ANALYZER_ALGORITHMOPENVINOYOLO8_H
#define ANALYZER_ALGORITHMOPENVINOYOLO8_H

#include <onnxruntime_cxx_api.h>
#include <mutex>
#include <queue>
#include <string>
#include <vector>
#include "Algorithm.h"

namespace AVSAnalyzer {
class Config;
class OnnxRuntimeEngine {
   public:
    explicit OnnxRuntimeEngine(std::string modelPath);
    ~OnnxRuntimeEngine();

   public:
    bool runInference(cv::Mat& image, std::vector<DetectObject>& detects);

   private:
    std::string mModelPath;
    Ort::Env mEnv{nullptr};
    Ort::SessionOptions mSessionOptions{nullptr};
    Ort::Session mSession{nullptr};
};
class AlgorithmOnnxRuntime : public Algorithm {
   public:
    AlgorithmOnnxRuntime(Config* config);
    virtual ~AlgorithmOnnxRuntime();

   public:
    virtual bool objectDetect(cv::Mat& image, std::vector<DetectObject>& detects);

   private:
    OnnxRuntimeEngine* mEngine;
};
}  // namespace AVSAnalyzer
#endif  //ANALYZER_ALGORITHMOPENVINOYOLO8_H
