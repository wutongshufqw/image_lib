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
            } catch (std::exception e) {
                std::cout << "错误: " << e.what() << std::endl;
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
            } catch (std::exception e) {
                std::cout << "错误: " << e.what() << std::endl;
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
                switch (key) {
                    case 1: {
                        std::cout << "请输入原图像名称: ";
                        std::string path;
                        std::cin >> path;
                        path = _path + "/input/" + path + ".bmp";
                        BMP bmp;
                        bmp.readBmp(path);
                        std::cout << "请输入滤波器大小: ";
                        int size;
                        std::cin >> size;
                        MeanFilter filter(size);
                        bmp = filter.apply(bmp);
                        std::cout << "请输入均值滤波器图像名称: ";
                        std::cin >> path;
                        path = _path + "/output/" + path + ".bmp";
                        bmp.writeBmp(path);
                        std::cout << "均值滤波器图像已保存至: " << path << std::endl;
                        break;
                    }
                    case 2: {
                        std::cout << "请输入原图像名称: ";
                        std::string path;
                        std::cin >> path;
                        path = _path + "/input/" + path + ".bmp";
                        BMP bmp;
                        bmp.readBmp(path);
                        std::cout << "请输入滤波器大小: ";
                        int size;
                        std::cin >> size;
                        MedianFilter filter(size);
                        bmp = filter.apply(bmp);
                        std::cout << "请输入中值滤波器图像名称: ";
                        std::cin >> path;
                        path = _path + "/output/" + path + ".bmp";
                        bmp.writeBmp(path);
                        std::cout << "中值滤波器图像已保存至: " << path << std::endl;
                        break;
                    }
                    default:
                        return;
                }
            } catch (std::exception e) {
                std::cout << "错误: " << e.what() << std::endl;
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
                        int point[8];
                        for (int i = 0; i < 8; i++)
                            std::cin >> point[i];
                        std::cout << "请输入中值滤波次数: ";
                        int times;
                        std::cin >> times;
                        BMP perspective = Exchange::perspective(bmp, point, times);
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
            } catch (std::exception e) {
                std::cout << "错误: " << e.what() << std::endl;
            }
            // 清空输入缓冲区
            std::cout << "按 ESC 以继续";
            menu.esc();
        }
    }
}

Program::Program(std::string path) {
    _path = path;
    _menus.push_back(Menu(4, new std::string[4] {
        "真彩图像转灰度图像",
        "灰度图像反色",
        "RGB三通道分离",
        "返回上一级"
    }, "BMP图像处理", "按 ENTER 以继续"));
    _menus.push_back(Menu(3, new std::string[3] {
        "直方图统计",
        "直方图均衡化",
        "返回上一级"
    }, "直方图处理", "按 ENTER 以继续"));
    _menus.push_back(Menu(3, new std::string[3] {
        "均值滤波器",
        "中值滤波器",
        "返回上一级"
    }, "图像滤波处理", "按 ENTER 以继续"));
    _menus.push_back(Menu(6, new std::string[6] {
        "图像缩放",
        "图像平移",
        "图像镜像",
        "图像旋转",
        "图像透视",
        "返回上一级"
    }, "图像变换", "按 ENTER 以继续"));
}

void Program::start(int key) {
    switch (key) {
        case 1:
            program::program1(_menus[0], _path);
            break;
        case 2:
            program::program2(_menus[1], _path);
            break;
        case 3:
            program::program3(_menus[2], _path);
            break;
        case 4:
            program::program4(_menus[3], _path);
            break;
        default:
            break;
    }
}