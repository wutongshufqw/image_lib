#ifndef REGION_GROW_H
#define REGION_GROW_H

#include <queue>
#include <cstring>
#include "bmp.h"

typedef struct point {
    int x;
    int y;
    std::string tag;
    point(int x, int y, std::string tag) : x(x), y(y), tag(tag) {}
} Point;

typedef struct region {
    int x;
    int y;
    int width;
    int height;
    region(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {}
} Region;

class RegionGrow {
    public:
        static BMP grow(BMP &bmp, int *point, int threshold); // 双点法区域生长
        static BMP grow(BMP &bmp, int base, int threshold); // 种子法区域生长(自动寻找种子点)
};

class RegionSplit {
    public:
        static BMP split(BMP &bmp, int threshold); // 区域分裂
};
#endif