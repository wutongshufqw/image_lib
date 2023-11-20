#ifndef FILTER_H
#define FILTER_H
#include <algorithm>
#include "bmp.h"

//  滤波器基类
class Filter {
public:
    Filter() = default;
    virtual ~Filter() = default;
    virtual void apply(BMP &image) = 0;
};

//  均值滤波器
class MeanFilter: public Filter {
private:
    int _size;
public:
    MeanFilter(int size);
    void apply(BMP &image);
};

//  中值滤波器
class MedianFilter: public Filter {
private:
    int _size;
public:
    MedianFilter(int size);
    void apply(BMP &image);
};

#endif