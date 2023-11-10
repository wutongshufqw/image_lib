#include "include/menu.h"

namespace menu {
    int getLength(const std::string &str) { // 获取字符串实际长度
        int length = 0;
        for (int i = 0; i < str.length(); i++) {
            if (str[i] < 0) {
                length += 2;
                i += 2;
            } else {
                length += 1;
            }
        }
        return length;
    }

    std::string getLine(int length, int type) { // 生成一行字符
        std::string str = "";
        for (int i = 0; i < length - 2; i++)
            str += "─";
        if (type == 1)
            str = "┌" + str + "┐";
        else if (type == 2)
            str = "├" + str + "┤";
        else if (type == 3)
            str = "└" + str + "┘";
        return str + "\n";
    }

    std::string alignCenter(std::string str, int length) { // 居中对齐, 最左最右填充字符
        // 1. 计算左右填充字符的个数
        // 中英文混合, 一个中文占两个字符
        int strLength = getLength(str);
        int left = (length - strLength) / 2;
        int right = length - strLength - left;
        // 2. 填充
        return "│" + std::string(left - 1, ' ') + str + std::string(right - 1, ' ') + "│\n";
    }

    std::string alignLeft(std::string *str, int size, int length, int select) { // 左对齐, 最右填充字符
        int *strLength = new int[size];
        int maxLength = 0;
        for (int i = 0; i < size; i++) {
            strLength[i] = getLength(str[i]);
            if (strLength[i] > maxLength)
                maxLength = strLength[i];
        }
        // 1. 计算编号的长度
        int numLength = getLength(std::to_string(size)) + 2;
        // 2. 计算左侧位移
        #if defined(_WIN32) || defined(_WIN64)
            int totalLeft = (length - maxLength - numLength - 3) / 2;
        #elif defined(__linux__)
            int totalLeft = (length - maxLength - numLength - 2) / 2;
        #endif
        // 3. 生成菜单
        std::string result;
        for (int i = 0; i < size; i++) {
            std::string tmp = "│";
            // 3.1 填充左侧
            tmp += std::string(totalLeft - 1, ' ');
            // 3.2 填充箭头
            if (i + 1 == select)
                tmp += "→ ";
            else
                tmp += "  ";
            // 3.3 填充编号
            std::string num = std::to_string(i + 1); // 3.3.1 转换为str
            if (num.length() < numLength - 2) // 3.3.2 左侧填充空格
                num = std::string(numLength - 2 - num.length(), ' ') + num;
            num += ". "; // 3.3.3 添加小数点
            tmp += num;
            // 3.4 菜单
            tmp += str[i];
            // 3.5 填充右侧
            tmp += std::string(length - totalLeft - numLength - getLength(str[i]) - 3, ' ');
            tmp += "│";
            result += tmp + "\n";
        }
        return result;
    }

    int scanKeyBoard() { // 获取键盘输入
        #if defined(_WIN32) || defined(_WIN64)
            return _getch();
        #elif defined(__linux__)
            int in;
            struct termios new_settings;
            struct termios stored_settings;
            tcgetattr(STDIN_FILENO, &stored_settings);
            new_settings = stored_settings;
            new_settings.c_lflag &= (~ICANON);
            new_settings.c_cc[VTIME] = 0;
            tcgetattr(STDIN_FILENO, &stored_settings);
            new_settings.c_cc[VMIN] = 1;
            tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);
            in = getchar();
            tcsetattr(STDIN_FILENO, TCSANOW, &stored_settings);
            return in;
        #endif
    }

    int getKeyBoard(bool esc = false) { // 获取键盘输入
        #if defined(_WIN32) || defined(_WIN64)
            switch (scanKeyBoard()) {
                case 13: // 回车
                    return ENTER;
                case 27: // ESC
                    return ESC;
                case 224: // 特殊键
                    switch(scanKeyBoard()) {
                        case 72: // 上
                            return UP;
                        case 75: // 左
                            return LEFT;
                        case 77: // 右
                            return RIGHT;
                        case 80: // 下
                            return DOWN;
                        default:
                            return 0;
                    }
                default:
                    return 0;
            }
        #elif defined(__linux__)
            switch (scanKeyBoard()) {
                case 10: // 回车
                    return ENTER;
                case 27: // 特殊键
                    if (esc)
                        return ESC;
                    switch(scanKeyBoard()) {
                        case 91: // 方向键
                            switch(scanKeyBoard()) {
                                case 65: // 上
                                    return UP;
                                case 66: // 下
                                    return DOWN;
                                case 67: // 右
                                    return RIGHT;
                                case 68: // 左
                                    return LEFT;
                                default:
                                    return 0;
                            }
                        default:
                            return 0;
                    }
                default:
                    return 0;
            }
        #endif
    }
}

int Menu::start() { // 开始菜单
    do {
        show();
        int key;
        while(!(key = menu::getKeyBoard()))
            show();
        switch (key) {
            case menu::UP:
                up();
                break;
            case menu::DOWN:
                down();
                break;
            case menu::ENTER:
                return _selected;
            default:
                break;
        }
    } while (true);
}

void Menu::show() { // 显示菜单
    clear();
    std::cout << menu::getLine(_width, 1);
    std::cout << menu::alignCenter(_title, _width);
    std::cout << menu::getLine(_width, 2);
    std::cout << menu::alignLeft(_options, _size, _width, _selected);
    if (_footer != "") {
        std::cout << menu::getLine(_width, 2);
        std::cout << menu::alignCenter(_footer, _width);
    }
    std::cout << menu::getLine(_width, 3);
}

void Menu::clear() { // 清空屏幕
    system(menu::CLEAR);
}

void Menu::up() { // 上移
    if (_selected > 1)
        _selected--;
    else
        _selected = _size;
}

void Menu::down() { // 下移
    if (_selected < _size)
        _selected++;
    else
        _selected = 1;
}

void Menu::esc() { // 读取ESC
    while (menu::getKeyBoard(true) != menu::ESC);
}