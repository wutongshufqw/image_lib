#ifndef MENU_H
#define MENU_H
#include <iostream>
#include <string>
#include <termio.h>
#include <unistd.h>

class Menu {
    private:
        int _width = 40;
        int _selected = 1;
        int _size;
        std::string* _options = nullptr;
        std::string _title = "";
        std::string _footer = "";
    public:
        Menu(int size, std::string* options, std::string title) {
            _size = size;
            _options = options;
            _title = title;
        };

        Menu(int size, std::string* options, std::string title, std::string footer) {
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