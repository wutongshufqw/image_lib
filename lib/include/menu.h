#ifndef MENU_H
#define MENU_H
#include <iostream>
#include <string>
#include <vector>

#if defined(_WIN32) || defined(_WIN64)
    #include <conio.h>
    #include <direct.h>
#elif defined(__linux__)
    #include <termios.h>
    #include <unistd.h>
#endif

namespace menu {
    const unsigned char UP = 1;
    const unsigned char DOWN = 2;
    const unsigned char LEFT = 3;
    const unsigned char RIGHT = 4;
    const unsigned char ENTER = 5;
    const unsigned char ESC = 6;
    #if defined(_WIN32) || defined(_WIN64)
        const char CLEAR[] = "cls";
    #elif defined(__linux__)
        const char CLEAR[] = "clear";
    #endif
}

class Menu {
    private:
        int _width = 40;
        int _selected = 1;
        int _size;
        std::vector<std::string> _options;
        std::string _title = "";
        std::string _footer = "";
    public:
        Menu(int size, std::vector<std::string> options, std::string title) {
            _size = size;
            _options = options;
            _title = title;
        };

        Menu(int size, std::vector<std::string> options, std::string title, std::string footer) {
            _size = size;
            _options = options;
            _title = title;
            _footer = footer;
        };

        ~Menu() {};

        int start();
        void show();
        void clear();
        void up();
        void down();
        void esc();
};
#endif