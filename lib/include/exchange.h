#ifndef EXCHANGE_H
#define EXCHANGE_H
#include "bmp.h"
#include "filter.h"
#include "include.h"

class Exchange {
    public:
        Exchange() = default;
        ~Exchange() = default;

        static BMP scale(BMP &image, float ratio); // 缩放
        static BMP translate(BMP &image, int x, int y); // 平移
        static BMP mirror(BMP &image, int mode); // 镜像
        static BMP rotate(BMP &image, int angle); // 旋转
        static BMP perspective(BMP &image, double right[8]); // 透视
};
#endif