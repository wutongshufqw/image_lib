#include "include/exchange.h"

BMP Exchange::scale(BMP &image, float ratio) { // 缩放
    int width = image.getInfoHeader().biWidth; // 图像宽度
    int height = image.getInfoHeader().biHeight; // 图像高度
    int newWidth = (int) width * ratio; // 新图像宽度
    int newHeight = (int) height * ratio; // 新图像高度
    int bitCount = image.getInfoHeader().biBitCount; // 颜色表大小
    BMP newImage(newWidth, newHeight, bitCount); // 新图像
    for (int i = 0; i < newWidth; i++) // 遍历新图像
        for (int j = 0; j < newHeight; j++) { // 遍历新图像
            if (bitCount == 24) { // 24位图像
                // 每三个字节为一组，分别缩放
                int x = (int) (i * 3 / ratio) / 3 * 3;
                int y = (int) (j / ratio);
                newImage.setPixel(i, j, image.getPixel(x, y)); // 设置新图像像素
            } else if (bitCount == 8) // 8位图像
                newImage.setPixel(i, j, *image.getPixel((int) (i / ratio), (int) (j / ratio))); // 设置新图像像素
            else
                throw "错误：不支持的颜色表大小";
        }
    return newImage; // 返回新图像
}

BMP Exchange::translate(BMP &image, int x, int y) {
    int width = image.getInfoHeader().biWidth; // 图像宽度
    int height = image.getInfoHeader().biHeight; // 图像高度
    int bitCount = image.getInfoHeader().biBitCount; // 颜色表大小
    BMP newImage(width, height, image.getInfoHeader().biBitCount); // 新图像
    for (int i = 0; i < width; i++) // 遍历图像
        for (int j = 0; j < height; j++) { // 遍历图像
            int x1 = (int) (i - x);
            int y1 = (int) (j + y);
            if (x1 < 0 || y1 < 0 || x1 >= width || y1 >= height)
                continue; // 越界
            if (bitCount == 24) { // 24位图像
                // 每三个字节为一组，分别平移
                newImage.setPixel(i, j, image.getPixel(x1 * 3, y1)); // 设置新图像像素
            } else if (bitCount == 8) // 8位图像
                newImage.setPixel(i, j, *image.getPixel(x1, y1)); // 设置新图像像素
            else
                throw "错误：不支持的颜色表大小";
        }
    return newImage; // 返回新图像
}

BMP Exchange::mirror(BMP &image, int mode) {
    int width = image.getInfoHeader().biWidth; // 图像宽度
    int height = image.getInfoHeader().biHeight; // 图像高度
    int bitCount = image.getInfoHeader().biBitCount; // 颜色表大小
    BMP newImage(width, height, bitCount); // 新图像
    for (int i = 0; i < width; i++) // 遍历图像
        for (int j = 0; j < height; j++) { // 遍历图像
            if (bitCount == 24) { // 24位图像
                // 每三个字节为一组，分别镜像
                int x = i * 3;
                int y = j;
                if (mode == 1)// 水平镜像
                    x = width * 3 - x - 3;
                else if (mode == 2) // 垂直镜像
                    y = height - y - 1;
                else if (mode == 3) { // 对角镜像
                    x = width * 3 - x - 3;
                    y = height - y - 1;
                } else
                    throw "错误：不支持的镜像模式";
                newImage.setPixel(i, j, image.getPixel(x, y)); // 设置新图像像素
            } else if (bitCount == 8) { // 8位图像
                int x = i;
                int y = j;
                if (mode == 1) // 水平镜像
                    x = width - x - 1;
                else if (mode == 2) // 垂直镜像
                    y = height - y - 1;
                else if (mode == 3) { // 对角镜像
                    x = width - x - 1;
                    y = height - y - 1;
                } else
                    throw "错误：不支持的镜像模式";
                newImage.setPixel(i, j, *image.getPixel(x, y)); // 设置新图像像素
            } else
                throw "错误：不支持的颜色表大小";
        }
    return newImage; // 返回新图像
}

BMP Exchange::rotate(BMP &image, int angle) {
    int width = image.getInfoHeader().biWidth; // 图像宽度
    int height = image.getInfoHeader().biHeight; // 图像高度
    int bitCount = image.getInfoHeader().biBitCount; // 颜色表大小
    // 旋转中心
    int centerX = width / 2;
    int centerY = height / 2;
    // 旋转角度
    double radian = angle * PI / 180;
    // 旋转后的图像大小
    int newWidth = (int) (width * fabs(cos(radian)) + height * fabs(sin(radian)));
    int newHeight = (int) (width * fabs(sin(radian)) + height * fabs(cos(radian)));
    // 旋转后的中心
    int newCenterX = newWidth / 2;
    int newCenterY = newHeight / 2;
    // 新图像
    BMP newImage(newWidth, newHeight, bitCount);
    for (int i = 0; i < newWidth; i++) // 遍历新图像
        for (int j = 0; j < newHeight; j++) { // 遍历新图像
            // 旋转后的坐标
            int x = (int) ((i - newCenterX) * cos(radian) - (j - newCenterY) * sin(radian) + centerX);
            int y = (int) ((i - newCenterX) * sin(radian) + (j - newCenterY) * cos(radian) + centerY);
            if (x < 0 || y < 0 || x >= width || y >= height)
                continue; // 越界
            if (bitCount == 24) { // 24位图像
                // 每三个字节为一组，分别旋转
                newImage.setPixel(i, j, image.getPixel(x * 3, y)); // 设置新图像像素
            } else if (bitCount == 8) // 8位图像
                newImage.setPixel(i, j, *image.getPixel(x, y)); // 设置新图像像素
            else
                throw "错误：不支持的颜色表大小";
        }
    return newImage; // 返回新图像
}

BMP Exchange::perspective(BMP &image, double right[8]) {
    int width = image.getInfoHeader().biWidth; // 图像宽度
    int height = image.getInfoHeader().biHeight; // 图像高度
    int bitCount = image.getInfoHeader().biBitCount; // 颜色表大小
    // 计算透视变换矩阵
    double left[8][8] = {
        {0, (double) height - 1, 1, 0, 0, 0, 0, (double) (1 - height) * right[0]},
        {0, 0, 0, 0, (double) height - 1, 1, 0, (double) (1 - height) * right[1]},
        {(double) width - 1, (double) height - 1, 1, 0, 0, 0, (double) (1 - width) * right[2], (double) (1 - height) * right[2]},
        {0, 0, 0, (double) width - 1, (double) height - 1, 1, (double) (1 - width) * right[3], (double) (1 - height) * right[3]},
        {0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0},
        {(double) width - 1, 0, 1, 0, 0, 0, (double) (1 - width) * right[6], 0},
        {0, 0, 0, (double) width - 1, 0, 1, (double) (1 - width) * right[7], 0}
    };
    double matrix[8];
    // 高斯消元法求解
    int row, col;
    double temp;
    for (row = 0, col = 0; row < 8 && col < 8; col++) {
        int max = col; // 主元
        for (int i = row + 1; i < 8; i++) // 找到最大主元
            if (fabs(left[i][col]) > fabs(left[max][col]))
                max = i;
        if (fabs(left[max][col]) < EPS) // 主元为0
            continue;
        if (max != row) { // 交换行
            for (int i = 0; i < 8; i++)
                std::swap(left[row][i], left[max][i]);
            std::swap(right[row], right[max]);
        }
        // 消元
        temp = left[row][col];
        for (int i = col; i < 8; i++)
            left[row][i] /= temp;
        right[row] /= temp;
        for (int i = row + 1; i < 8; i++)
            if (fabs(left[i][col]) > EPS) {
                temp = left[i][col];
                for (int j = col; j < 8; j++)
                    left[i][j] -= left[row][j] * temp;
                right[i] -= right[row] * temp;
            }
        row++;
    }
    if (row < 8) // 无解 or 无穷多解
        throw "错误：输入参数错误";
    // 回代
    for (int i = 7; i >= 0; i--) {
        matrix[i] = right[i];
        for (int j = i + 1; j < 8; j++)
            matrix[i] -= left[i][j] * matrix[j];
    }
    // 新图像
    BMP newImage(width, height, bitCount);
    // 将旧图像的每个像素点映射到新图像
    for (int i = 0; i < width; i++) // 遍历新图像
        for (int j = 0; j < height; j++) { // 遍历新图像
            int x = (int) (matrix[0] * i + matrix[1] * j + matrix[2]); // 旧图像x坐标
            int y = (int) (matrix[3] * i + matrix[4] * j + matrix[5]); // 旧图像y坐标
            if (x < 0 || y < 0 || x >= width || y >= height)
                continue; // 越界
            if (bitCount == 24) { // 24位图像
                // 每三个字节为一组，分别映射
                newImage.setPixel(i, j, image.getPixel(x * 3, y)); // 设置新图像像素
            } else if (bitCount == 8) // 8位图像
                newImage.setPixel(i, j, *image.getPixel(x, y)); // 设置新图像像素
            else
                throw "错误：不支持的颜色表大小";
        }
    return newImage; // 返回新图像
}