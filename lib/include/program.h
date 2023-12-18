#ifndef PROGRAM_H
#define PROGRAM_H

#include "bmp.h"
#include "contour.h"
#include "edge.h"
#include "exchange.h"
#include "filter.h"
#include "histogram.h"
#include "hough.h"
#include "include.h"
#include "menu.h"
#include "region.h"
#include "threshold.h"

#define PROGRAM_SIZE 11

class Program {
    private:
        std::string _path;
        std::vector<Menu> _menus;
    public:
        Program(std::string path, std::vector<std::string> titles);
        void start(int index);
};

class Batch {
    private:
        static std::vector<std::string> files;
        std::string _path;
        std::fstream _file;
        int index;

        std::string readString(std::vector<std::string> args, int index, std::string def = "");
        int readInt(std::vector<std::string> args, int index, int def = INT_INF);
        float readFloat(std::vector<std::string> args, int index, float def = FLOAT_INF);
        double readDouble(std::vector<std::string> args, int index, double def = DOUBLE_INF);

        int readChoose(std::vector<std::string> args, int index, int range, int def = INT_INF);
    public:
        Batch(std::string path, int index = 0);
        void start(std::string path);
};
#endif