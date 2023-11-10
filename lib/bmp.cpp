#include "include/bmp.h"

void BMP::readBmp(std::string filename) { // 读取位图
    std::ifstream ifile(filename, std::ios::binary);
    if (!ifile) {
        std::cout << "错误: 无法打开文件" << std::endl;
        return;
    }
    ifile.read((char *)&fhead, sizeof(BMPFILEHEADER)); // 读取文件头
    if (fhead.bfType != 0x4D42) { // 判断是否为bmp文件
        std::cout << "错误: 不是BMP文件" << std::endl;
        return;
    }
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
    colorTable = new RGBQUAD[colorTableSize];
    ifile.read((char *)colorTable, colorTableSize * sizeof(RGBQUAD));
    // 计算每行字节数
    lineByte = (ihead.biWidth * ihead.biBitCount / 8 + 3) / 4 * 4; // 4字节对齐
    // 读取位图数据
    bmpBuf = new BYTE[lineByte * ihead.biHeight];
    ifile.read((char *)bmpBuf, lineByte * ihead.biHeight);
    ifile.close();
    std::cout << "BMP 文件读取成功" << std::endl;
    std::cout << "文件大小: " << fhead.bfSize << std::endl;
    std::cout << "位图宽度: " << ihead.biWidth << std::endl;
    std::cout << "位图高度: " << ihead.biHeight << std::endl;
    std::cout << "位图位数: " << ihead.biBitCount << std::endl;
    std::cout << "调色板大小: " << colorTableSize << std::endl;
    std::cout << "每行字节数: " << lineByte << std::endl;
}

void BMP::writeBmp(std::string filename) { // 写入位图
    if (!bmpBuf) {
        std::cout << "错误: 位图数据为空" << std::endl;
        return;
    }
    std::ofstream ofile(filename, std::ios::binary);
    if (!ofile) {
        std::cout << "错误: 无法打开文件" << std::endl;
        return;
    }
    ofile.write((char *)&fhead, sizeof(BMPFILEHEADER)); // 写入文件头
    ofile.write((char *)&ihead, sizeof(BMPINFOHEADER)); // 写入信息头
    ofile.write((char *)colorTable, colorTableSize * sizeof(RGBQUAD)); // 写入调色板
    ofile.write((char *)bmpBuf, lineByte * ihead.biHeight); // 写入位图数据
    ofile.close();
}

void BMP::_grayScale() { // 灰度化(内部函数)
    if (ihead.biBitCount != 24) {
        std::cout << "错误: 不支持的位数" << std::endl;
        return;
    }
    if (!bmpBuf) {
        std::cout << "错误: 位图数据为空" << std::endl;
        return;
    }
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
    BYTE *grayBuf = new BYTE[lineByte * ihead.biHeight / 3];
    for (LONG i = 0; i < ihead.biHeight; i++)
        for (LONG j = 0; j < ihead.biWidth; j++) {
            gray = (int) (0.114 * bmpBuf[i * lineByte + j * 3] + 0.587 * bmpBuf[i * lineByte + j * 3 + 1] + 0.299 * bmpBuf[i * lineByte + j * 3 + 2]);
            grayBuf[i * ihead.biWidth + j] = gray;
        }
    // 更新每行字节数
    lineByte = (ihead.biWidth * 8 + 31) / 32 * 4;
    // 更新信息头
    ihead.biBitCount = 8;
    ihead.biSizeImage = lineByte * ihead.biHeight / 3;
    // 更新文件头
    fhead.bfSize = ihead.biSizeImage + sizeof(BMPFILEHEADER) + sizeof(BMPINFOHEADER) + sizeof(RGBQUAD) * 256;
    fhead.bfOffBits = sizeof(BMPFILEHEADER) + sizeof(BMPINFOHEADER) + sizeof(RGBQUAD) * 256;
    // 更新位图数据
    delete[] bmpBuf;
    bmpBuf = grayBuf;
    // 更新调色板大小
    colorTableSize = 256;
    std::cout << "灰度化成功" << std::endl;
}

BMP BMP::grayScale() { // 灰度化
    BMP bmp(*this);
    bmp._grayScale();
    return bmp;
}

void BMP::_reverseColor() { // 反色(内部函数)
    if (!bmpBuf) {
        std::cout << "错误: 位图数据为空" << std::endl;
        return;
    }
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
    else {
        std::cout << "错误: 不支持的位数" << std::endl;
        return;
    }
    std::cout << "反色成功" << std::endl;
}

BMP BMP::reverseColor() {
    BMP bmp(*this);
    bmp._reverseColor();
    return bmp;
}

BMP* BMP::splitColor() { // RGB三通道分离
    if (!bmpBuf) {
        std::cout << "错误: 位图数据为空" << std::endl;
        return NULL;
    }
    if (ihead.biBitCount != 24) {
        std::cout << "错误: 不支持的位数" << std::endl;
        return NULL;
    }
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
    std::cout << "RGB三通道分离成功" << std::endl;
    return bmp;
}