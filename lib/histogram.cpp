#include "include/histogram.h"

void Histogram::_statistics(bool output = false) { // 统计直方图
    if (!bmpBuf) {
        std::cout << "错误: 位图数据为空" << std::endl;
        return;
    }
    if (ihead.biBitCount != 8) {
        std::cout << "错误: 位图位数不为8" << std::endl;
        return;
    }
    if (_histogram) delete[] _histogram; // 防止内存泄漏
    _histogram = new int[256];
    memset(_histogram, 0, sizeof(int) * 256);
    int max = 0;
    for (int i = 0; i < ihead.biHeight; i++)
        for (int j = 0; j < ihead.biWidth; j++) {
            _histogram[bmpBuf[i * lineByte + j]]++;
            if (_histogram[bmpBuf[i * lineByte + j]] > max) 
                max = _histogram[bmpBuf[i * lineByte + j]];
        }
    // 归一化
    int *histogram = new int[256];
    int highest = 256;
    for (int i = 0; i < 256; i++)
        histogram[i] = (int) ((float) _histogram[i] * highest / max);
    if (output) {
        // 构建直方图
        ihead.biWidth = 256;
        ihead.biHeight = highest;
        lineByte = (ihead.biWidth * ihead.biBitCount / 8 + 3) / 4 * 4;
        ihead.biSizeImage = lineByte * ihead.biHeight;
        delete[] bmpBuf;
        bmpBuf = new BYTE[lineByte * ihead.biHeight];
        memset(bmpBuf, 255, sizeof(BYTE) * lineByte * ihead.biHeight);
        for (int i = 0; i < 256; i++)
            for (int j = 0; j < histogram[i]; j++)
                bmpBuf[j * lineByte + i] = 0;
        delete[] histogram;
        fhead.bfSize = lineByte * ihead.biHeight + fhead.bfOffBits;
    }
}

void Histogram::_balance() { // 均衡化处理
    _statistics();
    int *histogram = new int[256]; // 构建累计直方图
    int *map = new int[256]; // 构建映射表
    int sum = 0; // 累计直方图
    for (int i = 0; i < 256; i++) { // 计算累计直方图
        sum += _histogram[i]; // 累计直方图
        histogram[i] = sum; // 累计直方图
        map[i] = (int) ((float) histogram[i] * 255 / (ihead.biWidth * ihead.biHeight)); // 映射表
    }
    for (int i = 0; i < ihead.biHeight; i++) // 均衡化处理
        for (int j = 0; j < ihead.biWidth; j++)
            bmpBuf[i * lineByte + j] = map[bmpBuf[i * lineByte + j]];
    delete[] histogram;
    delete[] map;
}

Histogram::Histogram(const Histogram& histogram) : BMP(histogram) {
    if (histogram._histogram) {
        _histogram = new int[256];
        memcpy(_histogram, histogram._histogram, sizeof(int) * 256);
    }
}

Histogram::Histogram(const BMP& bmp) : BMP(bmp) {}

Histogram Histogram::statistics() {
    Histogram histogram(*this);
    histogram._statistics(true);
    return histogram;
}

Histogram Histogram::balance() {
    Histogram histogram(*this);
    histogram._balance();
    return histogram;
}

int* Histogram::getHistogram() {
    return _histogram;
}