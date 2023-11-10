#include <vector>
#include "menu.h"

class Program {
    private:
        std::vector<Menu> _menus;
    public:
        Program();
        void start(int index);
};