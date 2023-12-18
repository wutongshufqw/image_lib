#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "bmp.h"
#include "include.h"

class Histogram: public BMP {
    private:
        int* _histogram = nullptr;
        void _statistics(bool output);
        void _balance();
    public:
        Histogram() = default;
        Histogram(const Histogram& histogram);
        Histogram(const BMP& bmp);
        Histogram statistics();
        Histogram balance();

        int* getHistogram();
};
#endif