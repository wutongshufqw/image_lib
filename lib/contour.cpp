#include "include/contour.h"

Contour::Contour(BMP &origin) {
    if (origin.getBitCount() != 8)
        origin = origin.grayScale();
    this->origin = origin;
}

Contour Contour::extract() {
    int width = origin.getWidth();
    int height = origin.getHeight();
    BMP binarized = origin.binarize(); // 二值化
    result = BMP(width, height, 8); // 8位灰度图
    result.fill(255); // 白色背景
    for (int i = 1; i < width - 1; i++)
        for (int j = 1; j < height - 1; j++) {
            BYTE* window = binarized.getWindows(i - 1, j - 1, 3, 3);
            if (window[4] != 0)
                continue; // 中心像素不为黑色
            bool flag = false;
            for (int k = 0; k < 9; k++)
                if (k != 4 && window[k] == 255) {
                    flag = true;
                    break;
                }
            if (flag)
                result.setPixel(i, j, (BYTE) 0);
        }
    return *this;
}

BMP Contour::getResult() {
    return result;
}