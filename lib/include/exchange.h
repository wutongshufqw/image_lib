#ifndef EXCHANGE_H
#define EXCHANGE_H
#include <cmath>
#include "bmp.h"

#define PI acos(-1)

class Exchange {
public:
    Exchange() = default;
    ~Exchange() = default;

    static BMP scale(BMP &image, float ratio); // 缩放
    static BMP translate(BMP &image, int x, int y); // 平移
    static BMP mirror(BMP &image, int mode); // 镜像
    static BMP rotate(BMP &image, int angle); // 旋转
};
#endif