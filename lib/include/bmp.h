#ifndef BMP_H
#define BMP_H

#include "include.h"

#define WORD unsigned short
#define DWORD unsigned int
#define LONG unsigned int
#define BYTE unsigned char

#pragma pack(push, 2) // 2字节对齐
typedef struct tagBITMAPFILEHEADER { // 位图文件头
    WORD bfType; // 2 bytes, 文件类型, 必须为BM
    DWORD bfSize; // 4 bytes, 文件大小, 包含文件头和像素数据
    WORD bfReserved1; // 2 bytes, 保留, 必须为0
    WORD bfReserved2; // 2 bytes, 保留, 必须为0
    DWORD bfOffBits; // 4 bytes, 从文件头到像素数据的偏移量
} BMPFILEHEADER;

typedef struct tagBITMAPINFOHEADER { // 位图信息头
    DWORD biSize; // 4 bytes, 信息头大小, 必须为40
    LONG biWidth; // 4 bytes, 图像宽度, 单位为像素
    LONG biHeight; // 4 bytes, 图像高度, 单位为像素
    WORD biPlanes; // 2 bytes, 位平面数, 必须为1
    WORD biBitCount; // 2 bytes, 颜色位数, 1, 4, 8, 24
    DWORD biCompression; // 4 bytes, 压缩类型, 0: 不压缩, 1: RLE8, 2: RLE4, 3: Bitfields
    DWORD biSizeImage; // 4 bytes, 像素数据大小, 包括填充字节
    LONG biXPelsPerMeter; // 4 bytes, 水平分辨率, 像素/米
    LONG biYPelsPerMeter; // 4 bytes, 垂直分辨率, 像素/米
    DWORD biClrUsed; // 4 bytes, 颜色表中的颜色数, 0: 使用所有颜色
    DWORD biClrImportant; // 4 bytes, 重要颜色数, 0: 所有颜色都重要
} BMPINFOHEADER;

typedef struct tagRGBQUAD { // 位图颜色表
    BYTE rgbBlue; // 1 byte, 蓝色分量
    BYTE rgbGreen; // 1 byte, 绿色分量
    BYTE rgbRed; // 1 byte, 红色分量
    BYTE rgbReserved; // 1 byte, 保留, 必须为0
} RGBQUAD;
#pragma pack(pop)

class BMP {
    protected:
        int lineByte = 0; // 每行字节数
        BYTE *bmpBuf = nullptr; // 位图数据
        int colorTableSize = 0; // 调色板大小
        RGBQUAD *colorTable = nullptr; // 调色板
        BMPFILEHEADER fhead; // 文件头
        BMPINFOHEADER ihead; // 信息头
        void _grayScale(bool info = false); // 灰度化(内部函数)
        void _reverseColor(bool info = false); // 反色(内部函数)
    public:
        BMP(); // 默认构造函数
        BMP(const BMP &bmp); // 拷贝构造函数
        BMP(int width, int height, int bitCount = 24); // 创建空白画布
        ~BMP(); // 析构函数

        BMP& operator= (const BMP &bmp); // 赋值运算符重载

        BMP readBmp(std::string filename, bool info = false); // 读取位图
        BMP writeBmp(std::string filename, bool info = false); // 写入位图
        BMP grayScale(bool info = false); // 灰度化
        BMP grayToColor(bool info = false); // 灰度转彩色
        BMP reverseColor(bool info = false); // 反色
        BMP binarize(bool info = false); // 2值化
        BMP *splitColor(bool info = false); // 拆分RGB通道

        int getLineByte(); // 获取每行字节数
        BMPFILEHEADER getFileHeader(); // 获取文件头
        BMPINFOHEADER getInfoHeader(); // 获取信息头
        BYTE* getBmpBuf(); // 获取位图数据

        int getWidth(); // 获取宽度
        int getHeight(); // 获取高度
        int getBitCount(); // 获取颜色位数

        BYTE* getPixel(int x, int y); // 获取像素
        void setPixel(int x, int y, BYTE pixel); // 设置像素
        void setPixel(int x, int y, BYTE* pixel); // 设置像素
        void setPixel(int x, int y, BYTE red, BYTE green, BYTE blue); // 设置像素

        BYTE* getWindows(int x, int y, int width, int height); // 获取窗口

        void fill(BYTE pixel); // 填充
        void fill(BYTE* pixel); // 填充
        void fill(BYTE red, BYTE green, BYTE blue); // 填充

        void drawLine(int x1, int y1, int x2, int y2, BYTE pixel); // 画线段
        void drawLine(int x1, int y1, int x2, int y2, BYTE* pixel); // 画线段
        void drawLine(int x1, int y1, int x2, int y2, BYTE red, BYTE green, BYTE blue); // 画线段

        void drawStraightLine(int r, int theta, BYTE pixel); // 画直线
        void drawStraightLine(int r, int theta, BYTE* pixel); // 画直线
        void drawStraightLine(int r, int theta, BYTE red, BYTE green, BYTE blue); // 画直线
        
        void drawRect(int x, int y, int width, int height, BYTE pixel); // 画矩形
        void drawRect(int x, int y, int width, int height, BYTE* pixel); // 画矩形
        void drawRect(int x, int y, int width, int height, BYTE red, BYTE green, BYTE blue); // 画矩形
};
#endif