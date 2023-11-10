#include "lib/include/menu.h"
#include "lib/include/program.h"

int main() { // 主函数
    // 获取程序执行路径
    char path[256];
    #if defined(_WIN32) || defined(_WIN64)
        _getcwd(path, sizeof(path));
    #elif defined(__linux__)
        getcwd(path, sizeof(path));
    #endif
    std::string path_str = path;
    Program program(path_str); // 创建程序对象
    std::string title = "图像处理系统";
    std::string options[] = {
        "BMP图像处理",
        "退出"
    };
    std::string footer = "按 ENTER 以继续";
    Menu menu(sizeof(options) / sizeof(options[0]), options, title, footer);
    while (true) {
        int key = menu.start();
        if (key == 2) {
            menu.clear();
            break;
        } else
            program.start(key);
    }
    return 0;
}
