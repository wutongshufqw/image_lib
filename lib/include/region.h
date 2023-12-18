#ifndef REGION_H
#define REGION_H

#include "bmp.h"
#include "include.h"

typedef struct point {
    int x;
    int y;
    std::string tag;
    point(int x, int y) : x(x), y(y) {}
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

class EqualTable {
    private:
        std::unordered_map<int, std::vector<Point>> table;
        int count = 0;
    public:
        EqualTable();
        int add(int x, int y);
        int add(int x, int y, int count);
        int find(int x, int y);
        int merge(int a, int b);
        int getCount();
        std::vector<Point> getPoints(int count);
        std::unordered_map<int, std::vector<Point>> getTable();
};

class RegionLabel {
    private:
        BMP origin;
        BMP result;
    public:
        RegionLabel(BMP &origin);
        RegionLabel label();
        BMP getResult();
};
#endif