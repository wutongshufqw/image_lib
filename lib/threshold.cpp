#include "include/threshold.h"

HistogramThreshold::HistogramThreshold(BMP& bmp) {
    _bmp = bmp;
}

HistogramThresholdGiven::HistogramThresholdGiven(BMP& bmp): HistogramThreshold(bmp) {}

ThresholdType HistogramThresholdGiven::threshold() {
    std::cout << "请输入阈值T: ";
    int threshold;
    std::cin >> threshold;
    ThresholdType result;
    BMP histogram(256, 256, 24);
    Histogram hist(_bmp);
    hist = hist.statistics();
    int *_histogram = hist.getHistogram();
    // 归一化
    int *temp = new int[256];
    int max = 0;
    for (int i = 0; i < 256; i++)
        if (_histogram[i] > max)
            max = _histogram[i];
    for (int i = 0; i < 256; i++)
        temp[i] = (int) ((float) _histogram[i] * 256 / max);
    // 绘制直方图，给定的阈值用红线标出
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < temp[i]; j++)
            histogram.setPixel(i, j, 0, 0, 0);
        if (i == threshold)
            for (int j = 0; j < 256; j++)
                histogram.setPixel(i, j, 255, 0, 0);
    }
    result.histogram = histogram;
    // 记录阈值
    result.tsize = 1;
    result.thresholds = new int[1];
    result.thresholds[0] = threshold;
    // 分割结果
    result.bsize = 1;
    result.results = new BMP[1];
    int width = _bmp.getInfoHeader().biWidth;
    int height = _bmp.getInfoHeader().biHeight;
    result.results[0] = BMP(width, height, 8);
    // 分割
    for (int i = 0; i < width; i++) // 遍历图像
        for (int j = 0; j < height; j++) { // 遍历图像
            int gray = *_bmp.getPixel(i, j);
            if (gray < threshold)
                result.results[0].setPixel(i, j, (BYTE) 0);
            else
                result.results[0].setPixel(i, j, 255);
        }
    return result;
}

HistogramThresholdIterative::HistogramThresholdIterative(BMP& bmp): HistogramThreshold(bmp) {}

ThresholdType HistogramThresholdIterative::threshold() {
    ThresholdType result;
    BMP histogram(256, 256, 24);
    Histogram hist(_bmp);
    hist = hist.statistics();
    int *_histogram = hist.getHistogram();
    // 归一化
    int *temp = new int[256];
    int max = 0;
    for (int i = 0; i < 256; i++)
        if (_histogram[i] > max)
            max = _histogram[i];
    for (int i = 0; i < 256; i++)
        temp[i] = (int) ((float) _histogram[i] * 256 / max);
    result.tsize = 1;
    result.thresholds = new int[1];

    int width = _bmp.getInfoHeader().biWidth;
    int height = _bmp.getInfoHeader().biHeight;
    result.bsize = 1;
    result.results = new BMP[1];
    result.results[0] = BMP(width, height, 8);
    // 迭代阈值分割
    int threshold = 128;
    int lastThreshold = 0;
    while (abs(threshold - lastThreshold) > 1) {
        lastThreshold = threshold;
        int sum1 = 0, sum2 = 0, count1 = 0, count2 = 0;
        for (int i = 0; i < threshold; i++) {
            sum1 += i * _histogram[i];
            count1 += _histogram[i];
        }
        for (int i = threshold; i < 256; i++) {
            sum2 += i * _histogram[i];
            count2 += _histogram[i];
        }
        threshold = (int) ((float) (sum1 / count1 + sum2 / count2) / 2);
    }
    result.thresholds[0] = threshold;
    // 直方图标记
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < temp[i]; j++)
            histogram.setPixel(i, j, 0, 0, 0);
        if (i == threshold)
            for (int j = 0; j < 256; j++)
                histogram.setPixel(i, j, 255, 0, 0);
    }
    result.histogram = histogram;
    // 分割
    for (int i = 0; i < width; i++) // 遍历图像
        for (int j = 0; j < height; j++) { // 遍历图像
            int gray = *_bmp.getPixel(i, j);
            if (gray < threshold)
                result.results[0].setPixel(i, j, (BYTE) 0);
            else
                result.results[0].setPixel(i, j, 255);
        }
    return result;
}

HistogramThresholdOtsu::HistogramThresholdOtsu(BMP& bmp): HistogramThreshold(bmp) {}

ThresholdType HistogramThresholdOtsu::threshold() {
    ThresholdType result;
    BMP histogram(256, 256, 24);
    Histogram hist(_bmp);
    hist = hist.statistics();
    int *_histogram = hist.getHistogram();
    // 归一化
    int *temp = new int[256];
    int max = 0;
    for (int i = 0; i < 256; i++)
        if (_histogram[i] > max)
            max = _histogram[i];
    for (int i = 0; i < 256; i++)
        temp[i] = (int) ((float) _histogram[i] * 256 / max);
    result.tsize = 1;
    result.thresholds = new int[1];

    int width = _bmp.getInfoHeader().biWidth;
    int height = _bmp.getInfoHeader().biHeight;
    result.bsize = 1;
    result.results = new BMP[1];
    result.results[0] = BMP(width, height, 8);
    // Otsu阈值分割
    int threshold = 0;
    int maxVariance = 0;
    double pixel[256];
    for (int i = 0; i < 256; i++)
        pixel[i] = (double) _histogram[i] / (width * height);
    for (int i = 0; i < 256; i++) {
        double w0 = 0, w1 = 0, u0 = 0, u1 = 0;
        for (int j = 0; j < i; j++) { // 前景
            w0 += pixel[j];
            u0 += j * pixel[j];
        }
        for (int j = i; j < 256; j++) { // 背景
            w1 += pixel[j];
            u1 += j * pixel[j];
        }
        u0 /= w0;
        u1 /= w1;
        double variance = w0 * w1 * (u0 - u1) * (u0 - u1);
        if (variance > maxVariance) {
            maxVariance = (int) variance;
            threshold = i;
        }
    }
    result.thresholds[0] = threshold;
    // 直方图标记
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < temp[i]; j++)
            histogram.setPixel(i, j, 0, 0, 0);
        if (i == threshold)
            for (int j = 0; j < 256; j++)
                histogram.setPixel(i, j, 255, 0, 0);
    }
    result.histogram = histogram;
    // 分割
    for (int i = 0; i < width; i++) // 遍历图像
        for (int j = 0; j < height; j++) { // 遍历图像
            int gray = *_bmp.getPixel(i, j);
            if (gray < threshold)
                result.results[0].setPixel(i, j, (BYTE) 0);
            else
                result.results[0].setPixel(i, j, 255);
        }
    return result;
}