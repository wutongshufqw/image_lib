#ifndef HOUGH_H
#define HOUGH_H

#include "bmp.h"
#include "include.h"

class Hough {
    private:
        BMP origin;
        BMP result;
    public:
        Hough(BMP& origin);
        
        Hough lineDetect(double rate); // Hough变换, 检测直线

        BMP getResult();
};

#endif