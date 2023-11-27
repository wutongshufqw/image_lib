#include "include/exchange.h"

BMP Exchange::scale(BMP &image, float ratio) { // 缩放
    int width = image.getInfoHeader().biWidth; // 图像宽度
    int height = image.getInfoHeader().biHeight; // 图像高度
    int newWidth = width * ratio; // 新图像宽度
    int newHeight = height * ratio; // 新图像高度
    BMP newImage(newWidth, newHeight, image.getInfoHeader().biBitCount, false); // 新图像
    for (int i = 0; i < newHeight; i++) { // 遍历新图像
        for (int j = 0; j < newWidth; j++) { // 遍历新图像
            int x = j / ratio; // 原图像坐标
            int y = i / ratio; // 原图像坐标
            newImage.setPixel(j, i, image.getPixel(x, y)); // 设置新图像像素
        }
    }
    return newImage; // 返回新图像
}

BMP Exchange::translate(BMP &image, int x, int y) {
    int width = image.getInfoHeader().biWidth; // 图像宽度
    int height = image.getInfoHeader().biHeight; // 图像高度
    int colorTableSize = image.getInfoHeader().biBitCount; // 颜色表大小
    BMP newImage(width, height, image.getInfoHeader().biBitCount, false); // 新图像
    for (int i = 0; i < height; i++) { // 遍历图像
        int newY = i + y * colorTableSize / 8; // 新图像坐标
        if (newY < 0 || newY >= height) { 
            continue; // 越界
        }
        for (int j = 0; j < width; j++) { // 遍历图像
            int newX = j + x * colorTableSize / 8; // 新图像坐标
            if (newX < 0 || newX >= width) {
                continue; // 越界
            }
            newImage.setPixel(newX, newY, image.getPixel(j, i)); // 设置新图像像素
        }
    }
    return newImage; // 返回新图像
}

BMP Exchange::mirror(BMP &image, int mode) {
    int width = image.getInfoHeader().biWidth; // 图像宽度
    int height = image.getInfoHeader().biHeight; // 图像高度
    int colorTableSize = image.getInfoHeader().biBitCount; // 颜色表大小
    BMP newImage(width, height, image.getInfoHeader().biBitCount, false); // 新图像
    switch (mode) {
        case 1: // 水平镜像
            for (int i = 0; i < height; i++) { // 遍历图像
                for (int j = 0; j < width; j++) { // 遍历图像
                    if (colorTableSize == 24) { // 24位图像
                        // 每三个字节为一组，分别交换
                        if (j % 3 == 0) {
                            BYTE temp1[3] = {image.getPixel(j, i), image.getPixel(j + 1, i), image.getPixel(j + 2, i)};
                            BYTE temp2[3] = {image.getPixel(width - j - 3, i), image.getPixel(width - j - 2, i), image.getPixel(width - j - 1, i)};
                            newImage.setPixel(j, i, temp2[0]); // 设置新图像像素
                            newImage.setPixel(j + 1, i, temp2[1]); // 设置新图像像素
                            newImage.setPixel(j + 2, i, temp2[2]); // 设置新图像像素
                            newImage.setPixel(width - j - 3, i, temp1[0]); // 设置新图像像素
                            newImage.setPixel(width - j - 2, i, temp1[1]); // 设置新图像像素
                            newImage.setPixel(width - j - 1, i, temp1[2]); // 设置新图像像素
                        } else {
                            continue;
                        }
                    } else if (colorTableSize == 8) {
                        newImage.setPixel(j, i, image.getPixel(width - j - 1, i)); // 设置新图像像素
                        newImage.setPixel(width - j - 1, i, image.getPixel(j, i)); // 设置新图像像素
                    } else {
                        std::cout << "错误：不支持的颜色表大小" << std::endl;
                        break;
                    }
                }
            }
            break;
        case 2: // 垂直镜像
            for (int i = 0; i < height; i++) { // 遍历图像
                for (int j = 0; j < width; j++) { // 遍历图像
                    if (colorTableSize == 24) { // 24位图像
                        // 每三个字节为一组，分别交换
                        if (j % 3 == 0) {
                            BYTE temp1[3] = {image.getPixel(j, i), image.getPixel(j + 1, i), image.getPixel(j + 2, i)};
                            BYTE temp2[3] = {image.getPixel(j, height - i - 1), image.getPixel(j + 1, height - i - 1), image.getPixel(j + 2, height - i - 1)};
                            newImage.setPixel(j, i, temp2[0]); // 设置新图像像素
                            newImage.setPixel(j + 1, i, temp2[1]); // 设置新图像像素
                            newImage.setPixel(j + 2, i, temp2[2]); // 设置新图像像素
                            newImage.setPixel(j, height - i - 1, temp1[0]); // 设置新图像像素
                            newImage.setPixel(j + 1, height - i - 1, temp1[1]); // 设置新图像像素
                            newImage.setPixel(j + 2, height - i - 1, temp1[2]); // 设置新图像像素
                        } else {
                            continue;
                        }
                    } else if (colorTableSize == 8) {
                        newImage.setPixel(j, i, image.getPixel(j, height - i - 1)); // 设置新图像像素
                        newImage.setPixel(j, height - i - 1, image.getPixel(j, i)); // 设置新图像像素
                    } else {
                        std::cout << "错误：不支持的颜色表大小" << std::endl;
                        break;
                    }
                }
            }
            break;
        case 3: // 对角镜像
            for (int i = 0; i < height; i++) { // 遍历图像
                for (int j = 0; j < width; j++) { // 遍历图像
                    if (colorTableSize == 24) { // 24位图像
                        // 每三个字节为一组，分别交换
                        if (j % 3 == 0) {
                            BYTE temp1[3] = {image.getPixel(j, i), image.getPixel(j + 1, i), image.getPixel(j + 2, i)};
                            BYTE temp2[3] = {image.getPixel(width - j - 3, height - i - 1), image.getPixel(width - j - 2, height - i - 1), image.getPixel(width - j - 1, height - i - 1)};
                            newImage.setPixel(j, i, temp2[0]); // 设置新图像像素
                            newImage.setPixel(j + 1, i, temp2[1]); // 设置新图像像素
                            newImage.setPixel(j + 2, i, temp2[2]); // 设置新图像像素
                            newImage.setPixel(width - j - 3, height - i - 1, temp1[0]); // 设置新图像像素
                            newImage.setPixel(width - j - 2, height - i - 1, temp1[1]); // 设置新图像像素
                            newImage.setPixel(width - j - 1, height - i - 1, temp1[2]); // 设置新图像像素
                            continue;
                        }
                    } else if (colorTableSize == 8) {
                        newImage.setPixel(j, i, image.getPixel(i, j)); // 设置新图像像素
                        newImage.setPixel(i, j, image.getPixel(j, i)); // 设置新图像像素
                    } else {
                        std::cout << "错误：不支持的颜色表大小" << std::endl;
                        break;
                    }
                }
            }
            break;
        default:
            break;
    }
    return newImage; // 返回新图像
}

BMP Exchange::rotate(BMP &image, int angle) {
    int width = image.getInfoHeader().biWidth; // 图像宽度
    int height = image.getInfoHeader().biHeight; // 图像高度
    int colorTableSize = image.getInfoHeader().biBitCount; // 颜色表大小
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
    BMP newImage(newWidth, newHeight, image.getInfoHeader().biBitCount, false);
    for (int i = 0; i < newHeight; i++) { // 遍历新图像
        for (int j = 0; j < newWidth; j++) { // 遍历新图像
            // 原图像坐标
            int x = (int) ((j - newCenterX) * cos(radian) + (i - newCenterY) * sin(radian) + centerX);
            int y = (int) (-(j - newCenterX) * sin(radian) + (i - newCenterY) * cos(radian) + centerY);
            if (x < 0 || x >= width || y < 0 || y >= height) {
                continue; // 越界
            }
            newImage.setPixel(j, i, image.getPixel(x, y)); // 设置新图像像素
        }
    }
    return newImage; // 返回新图像
}