﻿#include "Core/Algorithm.h"
#include "Core/Config.h"
#include "Core/Scheduler.h"
#include "Core/Server.h"
#include "Core/Utils/Version.h"

using namespace AVSAnalyzer;

int main(int argc, char** argv) {
    // testRequest();
    // return 0;

#ifdef WIN32
    srand(time(NULL));  //时间初始化
#endif                  // WIN32

    const char* file = NULL;

    for (int i = 1; i < argc; i += 2) {
        if (argv[i][0] != '-') {
            printf("parameter error:%s\n", argv[i]);
            return -1;
        }
        switch (argv[i][1]) {
            case 'h': {
                //打印help信息
                printf("-h 打印参数配置信息并退出\n");
                printf("-f 配置文件    如：-f config.json \n");
                system("pause\n");
                exit(0);
                return -1;
            }
            case 'f': {
                file = argv[i + 1];
                break;
            }
            default: {
                printf("set parameter error:%s\n", argv[i]);
                return -1;
            }
        }
    }

    if (file == NULL) {
        printf("failed to read config file\n");
        return -1;
    }
    Config config(file);
    if (!config.mState) {
        printf("failed to read config file: %s\n", file);
        return -1;
    }

    printf("视频分析器 %s \n", PROJECT_VERSION);
    config.show();
    printf("\n");
    printf(
        "请注意! "
        "上面打印的配置参数中，有涉及路径的参数，一定要在config."
        "json修改成自己的路径，否则程序一定会报错的，如果不知道config."
        "json各个参数代表什么意思，请参考README.md\n");
    Scheduler scheduler(&config);
    if (!scheduler.initAlgorithm()) {
        return -1;
    }

    //测试dlib start
    // std::vector<DetectObject> detects;
    // Algorithm* algorithm = scheduler.gainAlgorithm();
    // algorithm->faceDataInit();
    // cv::Mat image = cv::imread("data/test1.jpg");
    // algorithm->faceDetect(image, true, detects);
    // algorithm->faceCalcuFeature(image, detects);
    // scheduler.giveBackAlgorithm(algorithm);
    //测试dlib end

    Server server;
    server.start(&scheduler);
    scheduler.loop();

    return 0;
}
