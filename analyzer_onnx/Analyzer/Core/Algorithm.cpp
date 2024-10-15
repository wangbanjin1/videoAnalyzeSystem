#include "Algorithm.h"
#include "Config.h"
#include "Utils/Common.h"
#include "Utils/Log.h"

namespace AVSAnalyzer {
Algorithm::Algorithm(Config* config) : mConfig(config) {}

Algorithm::~Algorithm() {}

}  // namespace AVSAnalyzer