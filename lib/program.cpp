#include "include/program.h"

namespace program {
    void program1(Menu menu, std::string _path) {
        while (true) {
            int key = menu.start();
            try {
                switch (key) {
                    case 1: {
                        std::cout << "请输入原图像名称: ";
                        std::string path;
                        std::cin >> path;
                        path = _path + "/input/" + path + ".bmp";
                        BMP bmp;
                        bmp.readBmp(path);
                        BMP gray = bmp.grayScale();
                        std::cout << "请输入灰度图像名称: ";
                        std::cin >> path;
                        path = _path + "/output/" + path + ".bmp";
                        gray.writeBmp(path);
                        std::cout << "灰度图像已保存至: " << path << std::endl;
                        break;
                    }
                    case 2: {
                        std::cout << "请输入原图像名称: ";
                        std::string path;
                        std::cin >> path;
                        path = _path + "/input/" + path + ".bmp";
                        BMP bmp;
                        bmp.readBmp(path);
                        BMP reverse = bmp.grayScale().reverseColor();
                        std::cout << "请输入反色图像名称: ";
                        std::cin >> path;
                        path = _path + "/output/" + path + ".bmp";
                        reverse.writeBmp(path);
                        std::cout << "反色图像已保存至: " << path << std::endl;
                        break;
                    }
                    case 3: {
                        std::cout << "请输入原图像名称: ";
                        std::string path;
                        std::cin >> path;
                        path = _path + "/input/" + path + ".bmp";
                        BMP bmp;
                        bmp.readBmp(path);
                        BMP* split = bmp.splitColor();
                        std::cout << "请输入三通道图像名称: ";
                        std::cin >> path;
                        for (int i = 0; i < 3; i++) {
                            BMP file = split[i];
                            std::string nowPath = _path + "/output/" + path + std::to_string(i) + ".bmp";
                            file.writeBmp(nowPath);
                            std::cout << "第" << i + 1 << "个通道图像已保存至: " << nowPath << std::endl;
                        }
                        break;
                    }
                    default:
                        return;
                }
            } catch (const char* msg) {
                std::cerr << msg << std::endl;
            }
            // 清空输入缓冲区
            std::cout << "按 ESC 以继续";
            menu.esc();
        }
    }

    void program2(Menu menu, std::string _path) {
        while (true) {
            int key = menu.start();
            try {
                switch (key) {
                    case 1: {
                        std::cout << "请输入原图像名称: ";
                        std::string path;
                        std::cin >> path;
                        path = _path + "/input/" + path + ".bmp";
                        Histogram hist;
                        hist.readBmp(path);
                        Histogram histogram = hist.statistics();
                        std::cout << "请输入直方图名称: ";
                        std::cin >> path;
                        path = _path + "/output/" + path + ".bmp";
                        histogram.writeBmp(path);
                        std::cout << "直方图已保存至: " << path << std::endl;
                        break;
                    }
                    case 2: {
                        std::cout << "请输入原图像名称: ";
                        std::string path;
                        std::cin >> path;
                        path = _path + "/input/" + path + ".bmp";
                        Histogram hist;
                        hist.readBmp(path);
                        Histogram histogram = hist.balance();
                        std::cout << "请输入均衡化图像名称: ";
                        std::cin >> path;
                        path = _path + "/output/" + path + ".bmp";
                        histogram.writeBmp(path);
                        std::cout << "均衡化图像已保存至: " << path << std::endl;
                        std::cout << "请输入均衡化直方图名称: ";
                        std::cin >> path;
                        path = _path + "/output/" + path + ".bmp";
                        histogram.statistics().writeBmp(path);
                        std::cout << "均衡化直方图已保存至: " << path << std::endl;
                        break;
                    }
                    default:
                        return;
                }
            } catch (const char* msg) {
                std::cerr << msg << std::endl;
            }
            // 清空输入缓冲区
            std::cout << "按 ESC 以继续";
            menu.esc();
        }
    }

    void program3(Menu menu, std::string _path) {
        while (true) {
            int key = menu.start();
            try {
                if (key == 3)
                    return;
                Filter *filter;
                std::cout << "请输入原图像名称: ";
                std::string path;
                std::cin >> path;
                path = _path + "/input/" + path + ".bmp";
                BMP bmp;
                bmp.readBmp(path);
                if (bmp.getInfoHeader().biBitCount != 8)
                    bmp = bmp.grayScale();
                std::cout << "请输入滤波卷积核大小: ";
                int size;
                std::cin >> size;
                switch (key) {
                    case 1:
                        filter = new MeanFilter(size);
                        break;
                    case 2:
                        filter = new MedianFilter(size);
                        break;
                    default:
                        return;
                }
                BMP result = filter->apply(bmp);
                std::cout << "请输入滤波图像名称: ";
                std::cin >> path;
                path = _path + "/output/" + path + ".bmp";
                result.writeBmp(path);
                std::cout << "滤波图像已保存至: " << path << std::endl;
            } catch (const char* msg) {
                std::cerr << msg << std::endl;
            }
            // 清空输入缓冲区
            std::cout << "按 ESC 以继续";
            menu.esc();
        }
    }

    void program4(Menu menu, std::string _path) {
        while (true) {
            int key = menu.start();
            try {
                switch (key) {
                    case 1: {
                        std::cout << "请输入原图像名称: ";
                        std::string path;
                        std::cin >> path;
                        path = _path + "/input/" + path + ".bmp";
                        BMP bmp;
                        bmp.readBmp(path);
                        std::cout << "请输入缩放比例: ";
                        float ratio;
                        std::cin >> ratio;
                        BMP scale = Exchange::scale(bmp, ratio);
                        std::cout << "请输入缩放图像名称: ";
                        std::cin >> path;
                        path = _path + "/output/" + path + ".bmp";
                        scale.writeBmp(path);
                        std::cout << "缩放图像已保存至: " << path << std::endl;
                        break;
                    }
                    case 2: {
                        std::cout << "请输入原图像名称: ";
                        std::string path;
                        std::cin >> path;
                        path = _path + "/input/" + path + ".bmp";
                        BMP bmp;
                        bmp.readBmp(path);
                        std::cout << "请输入平移距离[x, y]: ";
                        int x, y;
                        std::cin >> x >> y;
                        BMP translate = Exchange::translate(bmp, x, y);
                        std::cout << "请输入平移图像名称: ";
                        std::cin >> path;
                        path = _path + "/output/" + path + ".bmp";
                        translate.writeBmp(path);
                        std::cout << "平移图像已保存至: " << path << std::endl;
                        break;
                    }
                    case 3: {
                        std::cout << "请输入原图像名称: ";
                        std::string path;
                        std::cin >> path;
                        path = _path + "/input/" + path + ".bmp";
                        BMP bmp;
                        bmp.readBmp(path);
                        std::cout << "请输入镜像模式(1:水平, 2:垂直, 3:对角): ";
                        int mode;
                        std::cin >> mode;
                        BMP mirror = Exchange::mirror(bmp, mode);
                        std::cout << "请输入镜像图像名称: ";
                        std::cin >> path;
                        path = _path + "/output/" + path + ".bmp";
                        mirror.writeBmp(path);
                        std::cout << "镜像图像已保存至: " << path << std::endl;
                        break;
                    }
                    case 4: {
                        std::cout << "请输入原图像名称: ";
                        std::string path;
                        std::cin >> path;
                        path = _path + "/input/" + path + ".bmp";
                        BMP bmp;
                        bmp.readBmp(path);
                        std::cout << "请输入旋转角度(顺时针, 单位: °): ";
                        int angle;
                        std::cin >> angle;
                        BMP rotate = Exchange::rotate(bmp, angle);
                        std::cout << "请输入旋转图像名称: ";
                        std::cin >> path;
                        path = _path + "/output/" + path + ".bmp";
                        rotate.writeBmp(path);
                        std::cout << "旋转图像已保存至: " << path << std::endl;
                        break;
                    }
                    case 5: {
                        std::cout << "请输入原图像名称: ";
                        std::string path;
                        std::cin >> path;
                        path = _path + "/input/" + path + ".bmp";
                        BMP bmp;
                        bmp.readBmp(path);
                        std::cout << "请输入透视变换四个顶点坐标[x1, y1, x2, y2, x3, y3, x4, y4](顺序为左上, 右上, 左下, 右下): ";
                        double point[8];
                        for (int i = 0; i < 8; i++)
                            std::cin >> point[i];
                        BMP perspective = Exchange::perspective(bmp, point);
                        std::cout << "请输入透视变换图像名称: ";
                        std::cin >> path;
                        path = _path + "/output/" + path + ".bmp";
                        perspective.writeBmp(path);
                        std::cout << "透视变换图像已保存至: " << path << std::endl;
                        break;
                    }
                    default:
                        return;
                }
            } catch (const char* msg) {
                std::cerr << msg << std::endl;
            }
            // 清空输入缓冲区
            std::cout << "按 ESC 以继续";
            menu.esc();
        }
    }

    void program5(Menu menu, std::string _path) {
        while (true) {
            int key = menu.start();
            try {
                if (key == 4)
                    return;
                HistogramThreshold *threshold;
                std::cout << "请输入原图像名称: ";
                std::string path;
                std::cin >> path;
                path = _path + "/input/" + path + ".bmp";
                BMP bmp;
                bmp.readBmp(path);
                if (bmp.getInfoHeader().biBitCount != 8)
                    bmp = bmp.grayScale();
                switch (key) {
                    case 1:
                        threshold = new HistogramThresholdGiven(bmp);
                        break;
                    case 2:
                        threshold = new HistogramThresholdIterative(bmp);
                        break;
                    case 3:
                        threshold = new HistogramThresholdOtsu(bmp);
                        break;
                    default:
                        return;
                }
                ThresholdType result = threshold->threshold();
                std::cout << "请输入给定阈值直方图: ";
                std::cin >> path;
                path = _path + "/output/" + path + ".bmp";
                result.histogram.writeBmp(path);
                std::cout << "给定阈值直方图已保存至: " << path << std::endl;
                std::cout << "请输入给定阈值分割图像: ";
                std::cin >> path;
                path = _path + "/output/" + path + ".bmp";
                result.results[0].writeBmp(path);
                std::cout << "给定阈值分割图像已保存至: " << path << std::endl;
            } catch (const char* msg) {
                std::cerr << msg << std::endl;
            }
            // 清空输入缓冲区
            std::cout << "按 ESC 以继续";
            menu.esc();
        }
    }

    void program6(Menu menu, std::string _path) {
        while (true) {
            int key = menu.start();
            try {
                switch (key) {
                    case 1: {
                        std::cout << "请输入原图像名称: ";
                        std::string path;
                        std::cin >> path;
                        path = _path + "/input/" + path + ".bmp";
                        BMP bmp;
                        bmp.readBmp(path);
                        int mode = 0;
                        while (mode != 1 && mode != 2) {
                            std::cout << "请选择种子点模式[1:手动录入双点, 2:自动选取]: ";
                            std::cin >> mode;
                        }
                        BMP region;
                        if (mode == 1) {
                            int point[4];
                            std::cout << "请输入种子点坐标[x1, y1, x2, y2]: ";
                            std::cin >> point[0] >> point[1] >> point[2] >> point[3];
                            std::cout << "请输入阈值: ";
                            int threshold;
                            std::cin >> threshold;
                            region = RegionGrow::grow(bmp, point, threshold);
                        } else {
                            std::cout << "请输入种子点灰度值基准: ";
                            int base;
                            std::cin >> base;
                            std::cout << "请输入阈值: ";
                            int threshold;
                            std::cin >> threshold;
                            region = RegionGrow::grow(bmp, base, threshold);
                        }
                        std::cout << "请输入区域生长图像名称: ";
                        std::cin >> path;
                        path = _path + "/output/" + path + ".bmp";
                        region.writeBmp(path);
                        std::cout << "区域生长图像已保存至: " << path << std::endl;
                        break;
                    }
                    case 2: {
                        std::cout << "请输入原图像名称: ";
                        std::string path;
                        std::cin >> path;
                        path = _path + "/input/" + path + ".bmp";
                        BMP bmp;
                        bmp.readBmp(path);
                        std::cout << "请输入阈值: ";
                        int threshold;
                        std::cin >> threshold;
                        BMP newBmp = RegionSplit::split(bmp, threshold);
                        std::cout << "请输入区域分裂图像名称: ";
                        std::cin >> path;
                        path = _path + "/output/" + path + ".bmp";
                        newBmp.writeBmp(path);
                        std::cout << "区域分裂图像已保存至: " << path << std::endl;
                        break;
                    }
                    default:
                        return;
                }
            } catch (const char* msg) {
                std::cerr << msg << std::endl;
            }
            // 清空输入缓冲区
            std::cout << "按 ESC 以继续";
            menu.esc();
        }
    }

    void program7(Menu menu, std::string _path) {
        while (true) {
            int key = menu.start();
            try {
                std::cout << "请输入原图像名称: ";
                std::string path;
                std::cin >> path;
                path = _path + "/input/" + path + ".bmp";
                BMP bmp;
                bmp.readBmp(path);
                std::cout << "请输入输出阈值: ";
                int threshold;
                std::cin >> threshold;
                EdgeOperator *op;
                switch (key) {
                    case 1:
                        op = new PrewittOperator();
                        break;
                    case 2:
                        op = new SobelOperator();
                        break;
                    case 3:
                        op = new LOGOperator();
                        break;
                    default:
                        throw "未知的边缘检测算子";
                }
                BMP edge = Edge(op, bmp, threshold).detect().getEdge();
                std::cout << "请输入边缘检测图像名称: ";
                std::cin >> path;
                path = _path + "/output/" + path + ".bmp";
                edge.writeBmp(path);
                std::cout << "边缘检测图像已保存至: " << path << std::endl;
            } catch (const char* msg) {
                std::cerr << msg << std::endl;
            }
            // 清空输入缓冲区
            std::cout << "按 ESC 以继续";
            menu.esc();
        }
    }

    void program8(Menu menu, std::string _path) {
        while (true) {
            int key = menu.start();
            try {
                
            } catch (const char* msg) {
                std::cerr << msg << std::endl;
            }
            // 清空输入缓冲区
            std::cout << "按 ESC 以继续";
            menu.esc();
        }
    }

    // 函数指针数组
    void (*programs[8])(Menu, std::string) = {
        program1,
        program2,
        program3,
        program4,
        program5,
        program6,
        program7,
        program8
    };
}

Program::Program(std::string path, std::vector<std::string> titles) {
    std::vector<std::vector<std::string>> options = {
        {"真彩图像转灰度图像", "灰度图像反色", "RGB三通道分离", "返回上一级"},
        {"直方图统计", "直方图均衡化", "返回上一级"},
        {"均值滤波器", "中值滤波器", "返回上一级"},
        {"图像缩放", "图像平移", "图像镜像", "图像旋转", "图像透视", "返回上一级"},
        {"给定阈值T", "迭代阈值法", "Ostu", "返回上一级"},
        {"基于种子点进行区域生长", "区域分裂", "返回上一级"},
        {"Prewitt", "Sobel", "LOG", "返回上一级"},
        {"直线检测", "返回上一级"}
    };
    _path = path;
    for (int i = 0; i < options.size(); i++)
        _menus.push_back(Menu(options[i].size(), options[i], titles[i], "按 ENTER 以继续"));
}

void Program::start(int key) {
    if (key < 1 || key > 8)
        throw "发生错误：未知的程序";
    program::programs[key - 1](_menus[key - 1], _path);
}