#include "include/filter.h"

//  均值滤波器
MeanFilter::MeanFilter(int size) {
    _size = size;
}

BMP MeanFilter::apply(BMP &image) {
    BMP bmp;
    if (image.getInfoHeader().biBitCount != 8) // 仅支持灰度图像
        bmp = image.grayScale();
    else
        bmp = BMP(image);

    int width = bmp.getInfoHeader().biWidth; // 图像宽度
    int lineByte = bmp.getLineByte(); // 每行字节数
    int height = bmp.getInfoHeader().biHeight; // 图像高度
    BYTE *bmpBuf = bmp.getBmpBuf(); // 图像数据

    BYTE *temp = new BYTE[lineByte * height]; //  申请临时空间
    std::copy(bmpBuf, bmpBuf + lineByte * height, temp); //  拷贝图像数据

    int size = _size / 2; //  滤波器大小
    int sum = 0; //  累加和
    int count = 0; //  累加次数
    for (int i = size; i < height - size; i++) { //  遍历图像: i为行
        for (int j = size; j < width - size; j++) { //  遍历图像: j为列
            sum = 0; //  累加和
            count = 0; //  累加次数
            for (int k = -size; k <= size; k++) { //  遍历滤波器: k为行
                for (int l = -size; l <= size; l++) { //  遍历滤波器: l为列
                    int row = std::max(0, std::min(height - 1, i + k)); //  滤波器行坐标
                    int col = std::max(0, std::min(width - 1, j + l)); //  滤波器列坐标
                    sum += temp[row * lineByte + col]; //  累加和
                    count++; //  累加次数
                }
            }
            bmpBuf[i * lineByte + j] = sum / count;
        }
    }
    return bmp;
}

//  中值滤波器
MedianFilter::MedianFilter(int size) {
    _size = size;
}

BMP MedianFilter::apply(BMP &image) {
    BMP bmp;
    if (image.getInfoHeader().biBitCount != 8) // 仅支持灰度图像
        bmp = image.grayScale();
    else
        bmp = BMP(image);

    int width = bmp.getInfoHeader().biWidth; // 图像宽度
    int lineByte = bmp.getLineByte(); // 每行字节数
    int height = bmp.getInfoHeader().biHeight; // 图像高度
    BYTE *bmpBuf = bmp.getBmpBuf(); // 图像数据

    BYTE *temp = new BYTE[lineByte * height]; //  申请临时空间
    std::copy(bmpBuf, bmpBuf + lineByte * height, temp); //  拷贝图像数据

    int size = _size / 2; //  滤波器大小
    int count = 0; //  累计次数
    BYTE *arr = new BYTE[_size * _size]; //  申请临时空间
    for (int i = size; i < height - size; i++) { //  遍历图像: i为行
        for (int j = size; j < width - size; j++) { //  遍历图像: j为列
            count = 0; //  累计次数
            for (int k = -size; k <= size; k++) { //  遍历滤波器: k为行
                for (int l = -size; l <= size; l++) { //  遍历滤波器: l为列
                    int row = std::max(0, std::min(height - 1, i + k)); //  滤波器行坐标
                    int col = std::max(0, std::min(width - 1, j + l)); //  滤波器列坐标
                    arr[count++] = temp[row * lineByte + col]; //  累计次数
                }
            }
            std::sort(arr, arr + count); //  排序
            bmpBuf[i * lineByte + j] = arr[count / 2]; //  取中值
        }
    }
    return bmp;
}