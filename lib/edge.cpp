#include "include/edge.h"

namespace {
    double* gaussian(int size) { // 生成高斯模板
        double *g = new double[size * size];
        double sum = 0;
        for (int i = 0; i < size * size; i++) {
            int x = i / size - size / 2;
            int y = i % size - size / 2;
            g[i] = exp(-(x * x + y * y) / 2.0) / (2 * PI);
            sum += g[i];
        }
        for (int i = 0; i < size * size; i++)
            g[i] /= sum;
        return g;
    }
}

int PrewittOperator::gx(BYTE *data) { // 计算x方向梯度
    int model[9] = {
        -1, -1, -1,
         0,  0,  0,
         1,  1,  1
    };
    int sum = 0;
    for (int i = 0; i < 9; i++)
        sum += data[i] * model[i];
    return abs(sum);
}

int PrewittOperator::gy(BYTE *data) { // 计算y方向梯度
    int model[9] = {
        -1,  0,  1,
        -1,  0,  1,
        -1,  0,  1
    };
    int sum = 0;
    for (int i = 0; i < 9; i++)
        sum += data[i] * model[i];
    return abs(sum);
}

int SobelOperator::gx(BYTE *data) { // 计算x方向梯度
    int model[9] = {
        -1, -2, -1,
         0,  0,  0,
         1,  2,  1
    };
    int sum = 0;
    for (int i = 0; i < 9; i++)
        sum += data[i] * model[i];
    return abs(sum);
}

int SobelOperator::gy(BYTE *data) { // 计算y方向梯度
    int model[9] = {
        -1,  0,  1,
        -2,  0,  2,
        -1,  0,  1
    };
    int sum = 0;
    for (int i = 0; i < 9; i++)
        sum += data[i] * model[i];
    return abs(sum);
}

int LOGOperator::gx(BYTE *data) { // 计算x方向梯度
    int model[9] = {
         1,  1,  1,
         1, -8,  1,
         1,  1,  1
    };
    int sum = 0;
    for (int i = 0; i < 9; i++)
        sum += data[i] * model[i];
    return abs(sum);
}

int LOGOperator::gy(BYTE *data) { // 计算y方向梯度
    return gx(data);
}

Edge::Edge(EdgeOperator *op, BMP bmp, int threshold) { // 构造函数
    this->op = op;
    if (bmp.getBitCount() != 8)
        bmp = bmp.grayScale();
    this->bmp = bmp;
    this->threshold = threshold;
}

Edge::~Edge() { // 析构函数
}

BMP Edge::getEdge() { // 获取边缘图像
    return edge;
}

Edge Edge::detect() { // 边缘检测
    edge = BMP(bmp.getWidth(), bmp.getHeight(), 8); // 初始化边缘图像
    edge.fill((BYTE) 0); // 填充为黑色
    for (int i = 1; i < bmp.getHeight() - 1; i++) // 遍历图像
        for (int j = 1; j < bmp.getWidth() - 1; j++) {
            BYTE *data;
            // LOG特殊处理
            if (typeid(*op) == typeid(LOGOperator)) {
                if (i == 1 || i == bmp.getHeight() - 2 || j == 1 || j == bmp.getWidth() - 2)
                    continue;
                data = new BYTE[9];
                double *g = gaussian(3);
                for (int k = 0; k < 9; k++) {
                    int x = k / 3 - 2;
                    int y = k % 3 - 2;
                    BYTE *w = bmp.getWindows(j + x, i + y, 3, 3);
                    double sum = 0;
                    for (int l = 0; l < 9; l++)
                        sum += w[l] * g[l];
                    data[k] = (BYTE) sum;
                }
                delete[] g;
            } else
                data = bmp.getWindows(j - 1, i - 1, 3, 3);
            int gx = op->gx(data); // 计算x方向梯度
            int gy = op->gy(data); // 计算y方向梯度
            int g = (int) sqrt(gx * gx + gy * gy); // 计算梯度
            if (g > 255)
                g = 255;
            if (g >= threshold) // 边缘检测
                edge.setPixel(j, i, 255);
        }
    return *this;
}