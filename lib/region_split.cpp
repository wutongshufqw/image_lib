#include "include/region_split.h"

BMP RegionGrow::grow(BMP &bmp, int *point, int threshold) {
    // 如果不是灰度图像, 则先灰度化
    if (bmp.getInfoHeader().biBitCount != 8)
        bmp = bmp.grayScale();
    // 获取图像宽度和高度
    int width = bmp.getInfoHeader().biWidth;
    int height = bmp.getInfoHeader().biHeight;
    // 创建新的图像
    BMP newBmp(bmp);
    // 获取图像数据
    BYTE *bmpBuf = bmp.getBmpBuf();
    BYTE *newBmpBuf = newBmp.getBmpBuf();
    // 创建队列
    std::queue<Point> queue;
    // 将种子点入队
    queue.push(Point(point[0], point[1], "white"));
    queue.push(Point(point[2], point[3], "black"));
    // 访问标记数组
    bool *visited = new bool[width * height];
    std::memset(visited, false, width * height);
    // 当队列不为空时
    while (!queue.empty()) {
        // 出队
        Point point = queue.front();
        queue.pop();
        // 如果当前点未被访问
        if (!visited[point.y * width + point.x]) {
            // 标记为已访问
            visited[point.y * width + point.x] = true;
            // 设置当前点的像素值
            if (queue.front().tag == "white")
                newBmp.setPixel(point.x, point.y, 255);
            else
                newBmp.setPixel(point.x, point.y, (BYTE) 0);
            // 获取当前点的像素值
            BYTE pixel = *bmp.getPixel(point.x, point.y);
            // 遍历当前点的上下左右四个点
            for (int i = -1; i <= 1; i++)
                for (int j = -1; j <= 1; j++)
                    // 如果不是当前点
                    if (i != 0 || j != 0) {
                        // 计算新的坐标
                        int newX = point.x + i;
                        int newY = point.y + j;
                        // 如果新的坐标在图像范围内
                        if (newX >= 0 && newX < width && newY >= 0 && newY < height) {
                            if (visited[newY * width + newX])
                                continue;
                            // 获取新的像素值
                            BYTE newPixel = *bmp.getPixel(newX, newY);
                            // 如果新的像素值与当前像素值的差小于阈值
                            if (std::abs(newPixel - pixel) < threshold)
                                // 将新的坐标入队
                                queue.push(Point(newX, newY, point.tag));
                        }
                    }
        }
    }
    return newBmp;
}

BMP RegionGrow::grow(BMP &bmp, int base, int threshold) {
    // 如果不是灰度图像, 则先灰度化
    if (bmp.getInfoHeader().biBitCount != 8)
        bmp = bmp.grayScale();
    // 获取图像宽度和高度
    int width = bmp.getInfoHeader().biWidth;
    int height = bmp.getInfoHeader().biHeight;
    // 创建新的图像
    BMP newBmp(width, height, 8);
    newBmp.fill((BYTE) 0);
    // 获取图像数据
    BYTE *bmpBuf = bmp.getBmpBuf();
    BYTE *newBmpBuf = newBmp.getBmpBuf();
    // 创建队列
    std::queue<Point> queue;
    // 访问标记数组
    bool *visited = new bool[width * height];
    std::memset(visited, false, width * height);
    // 遍历图像, 寻找种子点
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
            if (*bmp.getPixel(i, j) > base)
                queue.push(Point(i, j, "white"));
    while (!queue.empty()) {
        // 出队
        Point point = queue.front();
        queue.pop();
        // 如果当前点未被访问
        if (!visited[point.y * width + point.x]) {
            // 标记为已访问
            visited[point.y * width + point.x] = true;
            // 设置当前点的像素值
            if (queue.front().tag == "white")
                newBmp.setPixel(point.x, point.y, 255);
            else
                newBmp.setPixel(point.x, point.y, (BYTE) 128);
            // 获取当前点的像素值
            BYTE pixel = *bmp.getPixel(point.x, point.y);
            // 遍历当前点的上下左右四个点
            for (int i = -1; i <= 1; i++)
                for (int j = -1; j <= 1; j++)
                    // 如果不是当前点
                    if (i != 0 || j != 0) {
                        // 计算新的坐标
                        int newX = point.x + i;
                        int newY = point.y + j;
                        // 如果新的坐标在图像范围内
                        if (newX >= 0 && newX < width && newY >= 0 && newY < height) {
                            if (visited[newY * width + newX])
                                continue;
                            // 获取新的像素值
                            BYTE newPixel = *bmp.getPixel(newX, newY);
                            // 如果新的像素值与当前像素值的差小于阈值
                            if (std::abs(newPixel - pixel) < threshold)
                                // 将新的坐标入队
                                queue.push(Point(newX, newY, "gray"));
                        }
                    }
        }
    }
    return newBmp;
}
    