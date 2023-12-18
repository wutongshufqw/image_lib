#ifndef CONTOUR_H
#define CONTOUR_H

#include "bmp.h"
#include "include.h"

class Contour {
    private:
        BMP origin;
        BMP result;
    public:
        Contour(BMP &origin);
        Contour extract();
        BMP getResult();
};

#endif