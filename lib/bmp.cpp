#include "include/bmp.h"

void BMP::_grayScale(bool info) { // 灰度化(内部函数)
    if (ihead.biBitCount != 24)
        throw "错误: 不支持的位数";
    if (!bmpBuf)
        throw "错误: 位图数据为空";
    int gray;
    // 计算调色板
    colorTable = new RGBQUAD[256];
    for (int i = 0; i < 256; i++) {
        colorTable[i].rgbBlue = i;
        colorTable[i].rgbGreen = i;
        colorTable[i].rgbRed = i;
        colorTable[i].rgbReserved = 0;
    }
    // 计算灰度图
    // 更新信息头
    ihead.biBitCount = 8;
    // 更新每行字节数
    int originalLineByte = lineByte; // 原始每行字节数
    lineByte = (ihead.biWidth * ihead.biBitCount / 8 + 3) / 4 * 4; // 4字节对齐
    ihead.biSizeImage = lineByte * ihead.biHeight;
    BYTE *grayBuf = new BYTE[lineByte * ihead.biHeight];
    for (LONG i = 0; i < ihead.biHeight; i++)
        for (LONG j = 0; j < ihead.biWidth; j++) {
            gray = (BYTE) (0.114 * bmpBuf[i * originalLineByte + j * 3] + 0.587 * bmpBuf[i * originalLineByte + j * 3 + 1] + 0.299 * bmpBuf[i * originalLineByte + j * 3 + 2]);
            grayBuf[i * lineByte + j] = gray;
        }
    // 更新文件头
    fhead.bfSize = ihead.biSizeImage + sizeof(BMPFILEHEADER) + sizeof(BMPINFOHEADER) + sizeof(RGBQUAD) * 256;
    fhead.bfOffBits = sizeof(BMPFILEHEADER) + sizeof(BMPINFOHEADER) + sizeof(RGBQUAD) * 256;
    // 更新位图数据
    delete[] bmpBuf;
    bmpBuf = grayBuf;
    // 更新调色板大小
    colorTableSize = 256;
    if (info)
        std::cout << "灰度化成功" << std::endl;
}

void BMP::_reverseColor(bool info) { // 反色(内部函数)
    if (!bmpBuf)
        throw "错误: 位图数据为空";
    if (ihead.biBitCount == 24)
        for (LONG i = 0; i < ihead.biHeight; i++)
            for (LONG j = 0; j < ihead.biWidth; j++) {
                bmpBuf[i * lineByte + j * 3] = 255 - bmpBuf[i * lineByte + j * 3];
                bmpBuf[i * lineByte + j * 3 + 1] = 255 - bmpBuf[i * lineByte + j * 3 + 1];
                bmpBuf[i * lineByte + j * 3 + 2] = 255 - bmpBuf[i * lineByte + j * 3 + 2];
            }
    else if (ihead.biBitCount == 8)
        for (LONG i = 0; i < ihead.biHeight; i++)
            for (LONG j = 0; j < ihead.biWidth; j++)
                bmpBuf[i * ihead.biWidth + j] = 255 - bmpBuf[i * ihead.biWidth + j];
    else if (ihead.biBitCount == 4)
        for (LONG i = 0; i < ihead.biHeight; i++)
            for (LONG j = 0; j < ihead.biWidth / 2; j++) {
                bmpBuf[i * ihead.biWidth / 2 + j] = 15 - bmpBuf[i * ihead.biWidth / 2 + j];
                bmpBuf[i * ihead.biWidth / 2 + j] = (bmpBuf[i * ihead.biWidth / 2 + j] << 4) + (15 - bmpBuf[i * ihead.biWidth / 2 + j]);
            }
    else if (ihead.biBitCount == 1)
        for (LONG i = 0; i < ihead.biHeight; i++)
            for (LONG j = 0; j < ihead.biWidth / 8; j++) {
                bmpBuf[i * ihead.biWidth / 8 + j] = 255 - bmpBuf[i * ihead.biWidth / 8 + j];
                bmpBuf[i * ihead.biWidth / 8 + j] = (bmpBuf[i * ihead.biWidth / 8 + j] << 7) + (255 - bmpBuf[i * ihead.biWidth / 8 + j]);
                bmpBuf[i * ihead.biWidth / 8 + j] = (bmpBuf[i * ihead.biWidth / 8 + j] << 6) + (255 - bmpBuf[i * ihead.biWidth / 8 + j]);
                bmpBuf[i * ihead.biWidth / 8 + j] = (bmpBuf[i * ihead.biWidth / 8 + j] << 5) + (255 - bmpBuf[i * ihead.biWidth / 8 + j]);
                bmpBuf[i * ihead.biWidth / 8 + j] = (bmpBuf[i * ihead.biWidth / 8 + j] << 4) + (255 - bmpBuf[i * ihead.biWidth / 8 + j]);
                bmpBuf[i * ihead.biWidth / 8 + j] = (bmpBuf[i * ihead.biWidth / 8 + j] << 3) + (255 - bmpBuf[i * ihead.biWidth / 8 + j]);
                bmpBuf[i * ihead.biWidth / 8 + j] = (bmpBuf[i * ihead.biWidth / 8 + j] << 2) + (255 - bmpBuf[i * ihead.biWidth / 8 + j]);
                bmpBuf[i * ihead.biWidth / 8 + j] = (bmpBuf[i * ihead.biWidth / 8 + j] << 1) + (255 - bmpBuf[i * ihead.biWidth / 8 + j]);
            }
    else
        throw "错误: 不支持的位数";
    if (info)
        std::cout << "反色成功" << std::endl;
}


BMP::BMP() { // 构造函数
    lineByte = 0;
    bmpBuf = nullptr;
    colorTableSize = 0;
    colorTable = nullptr;
}

BMP::BMP(const BMP &bmp) { // 拷贝构造函数
    lineByte = bmp.lineByte;
    bmpBuf = new BYTE[lineByte * bmp.ihead.biHeight];
    std::copy(bmp.bmpBuf, bmp.bmpBuf + lineByte * bmp.ihead.biHeight, bmpBuf);
    colorTableSize = bmp.colorTableSize;
    if (colorTableSize) {
        colorTable = new RGBQUAD[colorTableSize];
        std::copy(bmp.colorTable, bmp.colorTable + colorTableSize, colorTable);
    } else {
        colorTable = nullptr;
    }
    fhead = bmp.fhead;
    ihead = bmp.ihead;
}

BMP::BMP(int width, int length, int bitCount) { // 创建空白画布
    if (bitCount != 24 && bitCount != 8 && bitCount != 4 && bitCount != 1)
        throw "错误: 不支持的位数";
    // 实际大小
    int realWidth = width * bitCount / 8;
    int realLength = length;
    // 每行字节数
    lineByte = (realWidth + 3) / 4 * 4;
    // 位图数据
    bmpBuf = new BYTE[lineByte * realLength];
    // 调色板
    if (bitCount == 8 || bitCount == 4 || bitCount == 1) {
        colorTableSize = 1 << bitCount;
        colorTable = new RGBQUAD[colorTableSize];
        for (int i = 0; i < colorTableSize; i++) {
            colorTable[i].rgbBlue = i;
            colorTable[i].rgbGreen = i;
            colorTable[i].rgbRed = i;
            colorTable[i].rgbReserved = 0;
        }
    } else {
        colorTableSize = 0;
        colorTable = nullptr;
    }
    // 文件头
    fhead.bfType = 0x4D42;
    fhead.bfSize = lineByte * realLength + sizeof(BMPFILEHEADER) + sizeof(BMPINFOHEADER) + sizeof(RGBQUAD) * colorTableSize;
    fhead.bfReserved1 = 0;
    fhead.bfReserved2 = 0;
    fhead.bfOffBits = sizeof(BMPFILEHEADER) + sizeof(BMPINFOHEADER) + sizeof(RGBQUAD) * colorTableSize;
    // 信息头
    ihead.biSize = sizeof(BMPINFOHEADER);
    ihead.biWidth = width;
    ihead.biHeight = length;
    ihead.biPlanes = 1;
    ihead.biBitCount = bitCount;
    ihead.biCompression = 0;
    ihead.biSizeImage = lineByte * realLength;
    ihead.biXPelsPerMeter = 0;
    ihead.biYPelsPerMeter = 0;
    ihead.biClrUsed = 0;
    ihead.biClrImportant = 0;
    // 位图数据(白色填充)
    for (int i = 0; i < lineByte * realLength; i++)
        bmpBuf[i] = 255;
}

BMP::~BMP() { // 析构函数
}


BMP& BMP::operator= (const BMP &bmp) { // 赋值运算符重载
    if (this == &bmp)
        return *this;
    lineByte = bmp.lineByte;
    bmpBuf = new BYTE[lineByte * bmp.ihead.biHeight];
    std::copy(bmp.bmpBuf, bmp.bmpBuf + lineByte * bmp.ihead.biHeight, bmpBuf);
    colorTableSize = bmp.colorTableSize;
    if (colorTableSize) {
        colorTable = new RGBQUAD[colorTableSize];
        std::copy(bmp.colorTable, bmp.colorTable + colorTableSize, colorTable);
    } else {
        colorTable = nullptr;
    }
    fhead = bmp.fhead;
    ihead = bmp.ihead;
    return *this;
}
        

BMP BMP::readBmp(std::string filename, bool info) { // 读取位图
    std::ifstream ifile(filename, std::ios::binary);
    if (!ifile)
        throw "错误: 无法打开文件";
    ifile.read((char *)&fhead, sizeof(BMPFILEHEADER)); // 读取文件头
    if (fhead.bfType != 0x4D42) // 判断是否为bmp文件
        throw "错误: 不是BMP文件";
    ifile.read((char *)&ihead, sizeof(BMPINFOHEADER)); // 读取信息头
    // 根据位数计算调色板大小
    switch (ihead.biBitCount) {
    case 1:
        colorTableSize = 2;
        break;
    case 4:
        colorTableSize = 16;
        break;
    case 8:
        colorTableSize = 256;
        break;
    case 24:
        colorTableSize = 0;
        break;
    default:
        throw "Error: Unsupported bit count.";
    }
    // 读取调色板
    if (colorTableSize) {
        colorTable = new RGBQUAD[colorTableSize];
        ifile.read((char *)colorTable, colorTableSize * sizeof(RGBQUAD));
    } else {
        colorTable = nullptr;
    }
    // 计算每行字节数
    lineByte = (ihead.biWidth * ihead.biBitCount / 8 + 3) / 4 * 4; // 4字节对齐
    // 读取位图数据
    bmpBuf = new BYTE[lineByte * ihead.biHeight];
    ifile.read((char *)bmpBuf, lineByte * ihead.biHeight);
    ifile.close();
    if (info) {
        std::cout << "BMP 文件读取成功" << std::endl;
        std::cout << "文件大小: " << fhead.bfSize << std::endl;
        std::cout << "位图宽度: " << ihead.biWidth << std::endl;
        std::cout << "位图高度: " << ihead.biHeight << std::endl;
        std::cout << "位图位数: " << ihead.biBitCount << std::endl;
        std::cout << "位图大小: " << ihead.biSizeImage << std::endl;
        std::cout << "调色板大小: " << colorTableSize << std::endl;
        std::cout << "每行字节数: " << lineByte << std::endl;
    }
    return *this;
}

BMP BMP::writeBmp(std::string filename, bool info) { // 写入位图
    if (!bmpBuf)
        throw "错误: 位图数据为空";
    std::ofstream ofile(filename, std::ios::binary);
    if (!ofile)
        throw "错误: 无法打开文件";
    ofile.write((char *)&fhead, sizeof(BMPFILEHEADER)); // 写入文件头
    ofile.write((char *)&ihead, sizeof(BMPINFOHEADER)); // 写入信息头
    ofile.write((char *)colorTable, colorTableSize * sizeof(RGBQUAD)); // 写入调色板
    ofile.write((char *)bmpBuf, lineByte * ihead.biHeight); // 写入位图数据
    ofile.close();
    if (info) {
        std::cout << "BMP 文件读取成功" << std::endl;
        std::cout << "文件大小: " << fhead.bfSize << std::endl;
        std::cout << "位图宽度: " << ihead.biWidth << std::endl;
        std::cout << "位图高度: " << ihead.biHeight << std::endl;
        std::cout << "位图位数: " << ihead.biBitCount << std::endl;
        std::cout << "位图大小: " << ihead.biSizeImage << std::endl;
        std::cout << "调色板大小: " << colorTableSize << std::endl;
        std::cout << "每行字节数: " << lineByte << std::endl;
    }
    return *this;
}

BMP BMP::grayScale(bool info) { // 灰度化
    BMP bmp(*this);
    bmp._grayScale(info);
    return bmp;
}

BMP BMP::grayToColor(bool info) { // 灰度图转彩色图
    if (!bmpBuf)
        throw "错误: 位图数据为空";
    if (ihead.biBitCount != 8)
        throw "错误: 不支持的位数";
    BMP bmp(*this);
    // 更新信息头
    bmp.ihead.biBitCount = 24;
    // 更新每行字节数
    int originalLineByte = lineByte; // 原始每行字节数
    bmp.lineByte = (ihead.biWidth * bmp.ihead.biBitCount / 8 + 3) / 4 * 4; // 4字节对齐
    bmp.ihead.biSizeImage = bmp.lineByte * ihead.biHeight;
    BYTE *colorBuf = new BYTE[bmp.lineByte * ihead.biHeight];
    for (LONG i = 0; i < ihead.biHeight; i++)
        for (LONG j = 0; j < ihead.biWidth; j++) {
            colorBuf[i * bmp.lineByte + j * 3] = bmp.colorTable[bmpBuf[i * originalLineByte + j]].rgbBlue;
            colorBuf[i * bmp.lineByte + j * 3 + 1] = bmp.colorTable[bmpBuf[i * originalLineByte + j]].rgbGreen;
            colorBuf[i * bmp.lineByte + j * 3 + 2] = bmp.colorTable[bmpBuf[i * originalLineByte + j]].rgbRed;
        }
    bmp.colorTable = nullptr;
    // 更新文件头
    bmp.fhead.bfSize = bmp.ihead.biSizeImage + sizeof(BMPFILEHEADER) + sizeof(BMPINFOHEADER);
    bmp.fhead.bfOffBits = sizeof(BMPFILEHEADER) + sizeof(BMPINFOHEADER);
    // 更新位图数据
    delete[] bmp.bmpBuf;
    bmp.bmpBuf = colorBuf;
    // 更新调色板大小
    bmp.colorTableSize = 0;
    if (info)
        std::cout << "灰度图转彩色图成功" << std::endl;
    return bmp;
}

BMP BMP::reverseColor(bool info) {
    BMP bmp(*this);
    bmp._reverseColor(info);
    return bmp;
}

BMP BMP::binarize(bool info) {
    if (!bmpBuf)
        throw "错误: 位图数据为空";
    if (ihead.biBitCount != 8)
        _grayScale();
    BMP bmp(*this);
    int width = bmp.getWidth();
    int height = bmp.getHeight();
    BYTE* buf = bmp.getBmpBuf();
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            buf[i * width + j] = buf[i * width + j] > 127 ? 255 : 0;
    if (info)
        std::cout << "2值化成功" << std::endl;
    return bmp;
}

BMP* BMP::splitColor(bool info) { // RGB三通道分离
    if (!bmpBuf)
        throw "错误: 位图数据为空";
    if (ihead.biBitCount != 24)
        throw "错误: 不支持的位数";
    BMP *bmp = new BMP[3];
    for (int i = 0; i < 3; i++) {
        bmp[i] = BMP(*this);
        BYTE *buf = new BYTE[lineByte * ihead.biHeight / 3];
        for (LONG j = 0; j < ihead.biHeight; j++)
            for (LONG k = 0; k < ihead.biWidth; k++)
                buf[j * ihead.biWidth + k] = bmpBuf[j * lineByte + k * 3 + i];
        RGBQUAD *newColorTable = new RGBQUAD[256];
        for (int i = 0; i < 256; i++) {
            newColorTable[i].rgbBlue = i;
            newColorTable[i].rgbGreen = i;
            newColorTable[i].rgbRed = i;
            newColorTable[i].rgbReserved = 0;
        }
        delete[] bmp[i].colorTable;
        bmp[i].colorTable = newColorTable;
        bmp[i].lineByte = (ihead.biWidth * 8 + 31) / 32 * 4;
        bmp[i].ihead.biBitCount = 8;
        bmp[i].ihead.biSizeImage = bmp[i].lineByte * ihead.biHeight / 3;
        bmp[i].fhead.bfSize = bmp[i].ihead.biSizeImage + sizeof(BMPFILEHEADER) + sizeof(BMPINFOHEADER) + sizeof(RGBQUAD) * 256;
        bmp[i].fhead.bfOffBits = sizeof(BMPFILEHEADER) + sizeof(BMPINFOHEADER) + sizeof(RGBQUAD) * 256;
        delete[] bmp[i].bmpBuf;
        bmp[i].bmpBuf = buf;
        bmp[i].colorTableSize = 256;
    }
    if (info)
        std::cout << "RGB三通道分离成功" << std::endl;
    return bmp;
}


int BMP::getLineByte() { // 获取每行字节数
    return lineByte;
}

BMPFILEHEADER BMP::getFileHeader() { // 获取文件头
    return fhead;
}

BMPINFOHEADER BMP::getInfoHeader() { // 获取信息头
    return ihead;
}

BYTE* BMP::getBmpBuf() { // 获取位图数据
    return bmpBuf;
}


int BMP::getWidth() { // 获取宽度
    return ihead.biWidth;
}

int BMP::getHeight() { // 获取高度
    return ihead.biHeight;
}

int BMP::getBitCount() { // 获取颜色位数
    return ihead.biBitCount;
}


BYTE* BMP::getPixel(int x, int y) { // 获取像素
    return &bmpBuf[y * lineByte + x];
}

void BMP::setPixel(int x, int y, BYTE pixel) { // 设置像素
    bmpBuf[y * lineByte + x] = pixel;
}

void BMP::setPixel(int x, int y, BYTE* pixel) { // 设置像素
    setPixel(x, y, pixel[0], pixel[1], pixel[2]);
}

void BMP::setPixel(int x, int y, BYTE red, BYTE green, BYTE blue) { // 设置像素
    bmpBuf[y * lineByte + x * 3] = blue;
    bmpBuf[y * lineByte + x * 3 + 1] = green;
    bmpBuf[y * lineByte + x * 3 + 2] = red;
}


BYTE* BMP::getWindows(int x, int y, int width, int height) { // 获取窗口
    if (ihead.biBitCount == 8) { // 8位
        BYTE *buf = new BYTE[width * height];
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
                buf[i * width + j] = bmpBuf[(y + i) * ihead.biWidth + x + j];
        return buf;
    } else if (ihead.biBitCount == 24) { // 24位
        BYTE **buf = new BYTE*[width * height];
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++) {
                buf[i * width + j] = new BYTE[3];
                buf[i * width + j][0] = bmpBuf[(y + i) * lineByte + (x + j) * 3];
                buf[i * width + j][1] = bmpBuf[(y + i) * lineByte + (x + j) * 3 + 1];
                buf[i * width + j][2] = bmpBuf[(y + i) * lineByte + (x + j) * 3 + 2];
            }
        return (BYTE *)buf;
    } else
        throw "错误: 不支持的位数";
}


void BMP::fill(BYTE pixel) { // 填充
    for (int i = 0; i < lineByte * ihead.biHeight; i++)
        bmpBuf[i] = pixel;
}

void BMP::fill(BYTE* pixel) { // 填充
    fill(pixel[0], pixel[1], pixel[2]);
}

void BMP::fill(BYTE red, BYTE green, BYTE blue) { // 填充
    for (int i = 0; i < lineByte * ihead.biHeight; i += 3) {
        bmpBuf[i] = blue;
        bmpBuf[i + 1] = green;
        bmpBuf[i + 2] = red;
    }
}


void BMP::drawLine(int x1, int y1, int x2, int y2, BYTE pixel) { // 画线
    if (x1 == x2) {
        if (y1 > y2)
            std::swap(y1, y2);
        for (int i = y1; i <= y2; i++)
            setPixel(x1, i, pixel);
    } else if (y1 == y2) {
        if (x1 > x2)
            std::swap(x1, x2);
        for (int i = x1; i <= x2; i++)
            setPixel(i, y1, pixel);
    } else {
        double k = (double) (y2 - y1) / (x2 - x1);
        if (std::abs(k) <= 1) {
            if (x1 > x2) {
                std::swap(x1, x2);
                std::swap(y1, y2);
            }
            for (int i = x1; i <= x2; i++)
                setPixel(i, (int) (y1 + k * (i - x1)), pixel);
        } else {
            if (y1 > y2) {
                std::swap(x1, x2);
                std::swap(y1, y2);
            }
            for (int i = y1; i <= y2; i++)
                setPixel((int) (x1 + (i - y1) / k), i, pixel);
        }
    }
}

void BMP::drawLine(int x1, int y1, int x2, int y2, BYTE* pixel) { // 画线
    drawLine(x1, y1, x2, y2, pixel[0], pixel[1], pixel[2]);
}

void BMP::drawLine(int x1, int y1, int x2, int y2, BYTE red, BYTE green, BYTE blue) { // 画线
    if (x1 == x2) {
        if (y1 > y2)
            std::swap(y1, y2);
        for (int i = y1; i <= y2; i++)
            setPixel(x1, i, red, green, blue);
    } else if (y1 == y2) {
        if (x1 > x2)
            std::swap(x1, x2);
        for (int i = x1; i <= x2; i++)
            setPixel(i, y1, red, green, blue);
    } else {
        double k = (double) (y2 - y1) / (x2 - x1);
        if (std::abs(k) <= 1) {
            if (x1 > x2) {
                std::swap(x1, x2);
                std::swap(y1, y2);
            }
            for (int i = x1; i <= x2; i++)
                setPixel(i, (int) (y1 + k * (i - x1)), red, green, blue);
        } else {
            if (y1 > y2) {
                std::swap(x1, x2);
                std::swap(y1, y2);
            }
            for (int i = y1; i <= y2; i++)
                setPixel((int) (x1 + (i - y1) / k), i, red, green, blue);
        }
    }
}


void BMP::drawStraightLine(int r, int theta, BYTE pixel) { // 画直线
    for (int i = 0; i < ihead.biWidth; i++)
        for (int j = 0; j < ihead.biHeight; j++)
            if (r == (int) ((i * cos(theta * PI / 180) + j * sin(theta * PI / 180)) * 10))
                setPixel(i, j, pixel);
}

void BMP::drawStraightLine(int r, int theta, BYTE* pixel) { // 画直线
    drawStraightLine(r, theta, pixel[0], pixel[1], pixel[2]);
}

void BMP::drawStraightLine(int r, int theta, BYTE red, BYTE green, BYTE blue) { // 画直线
    for (int i = 0; i < ihead.biWidth; i++)
        for (int j = 0; j < ihead.biHeight; j++)
            if (r == (int) ((i * cos(theta * PI / 180) + j * sin(theta * PI / 180)) * 10))
                setPixel(i, j, red, green, blue);
}


void BMP::drawRect(int x, int y, int width, int height, BYTE pixel) { // 画矩形
    drawLine(x, y, x + width, y, pixel);
    drawLine(x, y, x, y + height, pixel);
    drawLine(x + width, y, x + width, y + height, pixel);
    drawLine(x, y + height, x + width, y + height, pixel);
}

void BMP::drawRect(int x, int y, int width, int height, BYTE* pixel) { // 画矩形
    drawRect(x, y, width, height, pixel[0], pixel[1], pixel[2]);
}

void BMP::drawRect(int x, int y, int width, int height, BYTE red, BYTE green, BYTE blue) { // 画矩形
    drawLine(x, y, x + width, y, red, green, blue);
    drawLine(x, y, x, y + height, red, green, blue);
    drawLine(x + width, y, x + width, y + height, red, green, blue);
    drawLine(x, y + height, x + width, y + height, red, green, blue);
}