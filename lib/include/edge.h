#ifndef EDGE_H
#define EDGE_H
#include <cmath>
#include "bmp.h"

class EdgeOperator { // 边缘检测算子
    public:
        EdgeOperator() = default; // 默认构造函数
        ~EdgeOperator() = default; // 析构函数
        virtual int gx(BYTE *data) = 0; // 计算x方向梯度
        virtual int gy(BYTE *data) = 0; // 计算y方向梯度
};

class SobelOperator : public EdgeOperator { // Sobel算子
    public:
        SobelOperator() : EdgeOperator() {} // 构造函数
        ~SobelOperator() {} // 析构函数
        int gx(BYTE *data); // 计算x方向梯度
        int gy(BYTE *data); // 计算y方向梯度
};

class PrewittOperator : public EdgeOperator { // Prewitt算子
    public:
        PrewittOperator() : EdgeOperator() {} // 构造函数
        ~PrewittOperator() {} // 析构函数
        int gx(BYTE *data); // 计算x方向梯度
        int gy(BYTE *data); // 计算y方向梯度
};

class LOGOperator : public EdgeOperator { // LOG算子
    public:
        LOGOperator() : EdgeOperator() {} // 构造函数
        ~LOGOperator() {}; // 析构函数
        int gx(BYTE *data); // 计算x方向梯度
        int gy(BYTE *data); // 计算y方向梯度
};

class Edge { // 边缘检测
    private:
        EdgeOperator *op; // 边缘检测算子
        int threshold; // 阈值
        BMP bmp; // BMP图像
        BMP edge; // 边缘图像
    public:
        Edge(EdgeOperator *op, BMP bmp, int threshold = 0); // 构造函数
        ~Edge(); // 析构函数
        BMP getEdge(); // 获取边缘图像
        Edge detect(); // 边缘检测
};
#endif