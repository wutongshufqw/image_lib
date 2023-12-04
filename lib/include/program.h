#ifndef PROGRAM_H
#define PROGRAM_H
#include <list>
#include <vector>
#include "bmp.h"
#include "exchange.h"
#include "filter.h"
#include "histogram.h"
#include "menu.h"
#include "region_split.h"
#include "threshold.h"

class Program {
    private:
        std::string _path;
        std::vector<Menu> _menus;
    public:
        Program(std::string);
        void start(int index);
};
#endif