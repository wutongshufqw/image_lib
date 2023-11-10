#include <vector>
#include "menu.h"

class Program {
    private:
        std::string _path;
        std::vector<Menu> _menus;
    public:
        Program(std::string);
        void start(int index);
};