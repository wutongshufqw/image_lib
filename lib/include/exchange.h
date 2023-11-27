#ifndef EXCHANGE_H
#define EXCHANGE_H
#include <cmath>
#include "bmp.h"
#include "filter.h"

#define PI acos(-1)
#define EPS 1e-6

class Exchange {
public:
    Exchange() = default;
    ~Exchange() = default;

    static BMP scale(BMP &image, float ratio); // 缩放
    static BMP translate(BMP &image, int x, int y); // 平移
    static BMP mirror(BMP &image, int mode); // 镜像
    static BMP rotate(BMP &image, int angle); // 旋转
    static BMP perspective(BMP &image, int point[8], int times); // 透视
};
#endif