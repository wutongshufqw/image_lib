#include <cstring>
#include "bmp.h"

class Histogram: public BMP {
    private:
        int* _histogram = nullptr;
        void _statistics(bool output);
        void _balance();
    public:
        Histogram() = default;
        Histogram(const Histogram& histogram);
        Histogram statistics();
        Histogram balance();
};