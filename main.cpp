#include "lib/include/menu.h"
#include "lib/include/program.h"

int main() { // 主函数
    // 获取程序执行路径
    char buf[256];
    #if defined(_WIN32) || defined(_WIN64)
        _getcwd(path, sizeof(path));
        // 解决中文乱码问题
        system("chcp 65001");
    #elif defined(__linux__)
        getcwd(buf, sizeof(buf));
    #endif
    std::string path_str = buf;
    std::string title = "图像处理系统";
    std::vector<std::string> options = {
        "BMP图像处理",
        "直方图处理",
        "图像滤波处理",
        "图像变换",
        "阈值分割",
        "基于区域的分割",
        "边缘检测",
        "Hough变换",
        "退出"
    };
    std::string footer = "按 ENTER 以继续";
    Menu menu(options.size(), options, title, footer);
    Program program(path_str, options); // 创建程序对象
    while (true) {
        int key = menu.start();
        if (key == options.size()) {
            menu.clear();
            break;
        } else
            program.start(key);
    }
    return 0;
}
