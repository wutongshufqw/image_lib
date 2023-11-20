#ifndef EXCHANGE_H
#define EXCHANGE_H
#include "bmp.h"

class Exchange {
public:
    Exchange() = default;
    ~Exchange() = default;

    // 缩放
    static BMP scale(BMP &image, float ratio);
};
#endif