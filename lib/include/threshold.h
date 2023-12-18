#ifndef THRESHOLD_H
#define THRESHOLD_H

#include "histogram.h"

typedef struct thresholdType {
    BMP histogram;
    int tsize;
    int *thresholds;
    int bsize;
    BMP *results;
} ThresholdType;

class HistogramThreshold { // 阈值分割基类
    protected:
        BMP _bmp;
    public:
        HistogramThreshold(BMP& bmp);
        virtual ThresholdType threshold(int threshold = 0) = 0;
};

class HistogramThresholdGiven: public HistogramThreshold { // 给定阈值分割
    public:
        HistogramThresholdGiven(BMP& bmp);
        ThresholdType threshold(int threshold = 0);
};

class HistogramThresholdIterative: public HistogramThreshold { // 迭代阈值分割
    public:
        HistogramThresholdIterative(BMP& bmp);
        ThresholdType threshold(int threshold = 0);
};

class HistogramThresholdOtsu: public HistogramThreshold { // Otsu阈值分割
    public:
        HistogramThresholdOtsu(BMP& bmp);
        ThresholdType threshold(int threshold = 0);
};

#endif