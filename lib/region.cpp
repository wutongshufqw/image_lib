#include "include/region.h"

namespace region_color {
    // 获取从赤橙黄绿青蓝紫颜色
    BYTE color[7][3] = {
        {255, 0, 0},
        {255, 127, 0},
        {255, 255, 0},
        {0, 255, 0},
        {0, 255, 255},
        {0, 0, 255},
        {255, 0, 255}
    };
}

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
    
BMP RegionSplit::split(BMP &bmp, int threshold) {
    // 如果不是灰度图像, 则先灰度化
    if (bmp.getInfoHeader().biBitCount != 8)
        bmp = bmp.grayScale();
    // 获取图像宽度和高度
    int width = bmp.getInfoHeader().biWidth;
    int height = bmp.getInfoHeader().biHeight;
    // 创建新的图像
    BMP newBmp(bmp);
    // 创建队列
    std::queue<Region> queue;
    // 将整个图像入队
    queue.push(Region(0, 0, width, height));
    // 创建结果数组
    std::vector<Region> regions;
    // 当队列不为空时
    while (!queue.empty()) {
        // 出队
        Region region = queue.front();
        queue.pop();
        // 最大值和最小值
        BYTE max = 0, min = 255;
        // 遍历区域内的所有像素
        for (int i = region.x; i < region.x + region.width; i++)
            for (int j = region.y; j < region.y + region.height; j++) {
                // 获取像素值
                BYTE pixel = *bmp.getPixel(i, j);
                // 更新最大值和最小值
                if (pixel > max)
                    max = pixel;
                if (pixel < min)
                    min = pixel;
            }
        // 如果最大值和最小值的差大于阈值
        if (max - min > threshold) {
            // 将区域分裂为四个子区域
            queue.push(Region(region.x, region.y, region.width / 2, region.height / 2));
            queue.push(Region(region.x + region.width / 2, region.y, region.width / 2, region.height / 2));
            queue.push(Region(region.x, region.y + region.height / 2, region.width / 2, region.height / 2));
            queue.push(Region(region.x + region.width / 2, region.y + region.height / 2, region.width / 2, region.height / 2));
        } else {
            // 将区域加入结果数组
            regions.push_back(region);
        }
    }
    // 遍历结果数组
    for (int i = 0; i < regions.size(); i++) {
        // 获取区域
        Region region = regions[i];
        // 绘制区域
        newBmp.drawRect(region.x, region.y, region.width, region.height, (BYTE) 0);
    }
    return newBmp;
}

EqualTable::EqualTable() {}

int EqualTable::add(int x, int y) {
    count++;
    table[count] = std::vector<Point>();
    table[count].push_back(Point(x, y));
    return count;
}

int EqualTable::add(int x, int y, int count) {
    table[count].push_back(Point(x, y));
    return count;
}

int EqualTable::find(int x, int y) {
    for (auto it = table.begin(); it != table.end(); it++)
        for (int i = 0; i < it->second.size(); i++)
            if (it->second[i].x == x && it->second[i].y == y)
                return it->first;
    return -1;
}

int EqualTable::merge(int a, int b) {
    if (a == b)
        return a;
    else if (a > b)
        std::swap(a, b);
    for (int i = 0; i < table[b].size(); i++)
        table[a].push_back(table[b][i]);
    table.erase(b);
    return a;
}

std::unordered_map<int, std::vector<Point>> EqualTable::getTable() {
    return table;
}

std::vector<Point> EqualTable::getPoints(int count) {
    return table[count];
}

int EqualTable::getCount() {
    return count;
}

RegionLabel::RegionLabel(BMP &origin) {
    if (origin.getInfoHeader().biBitCount != 8)
        origin = origin.grayScale();
    this->origin = origin;
}

RegionLabel RegionLabel::label() {
    // 获取图像宽度和高度
    int width = origin.getInfoHeader().biWidth;
    int height = origin.getInfoHeader().biHeight;
    // 图像2值化
    origin = origin.binarize();
    // 构建等价表
    EqualTable table;
    // 遍历图像
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++) {
            if (*origin.getPixel(i, j) != 255) // 如果当前像素不是白色
                continue;
            if (i == 0 && j == 0) { // 如果是第一个像素
                table.add(i, j); // 添加到等价表
                continue;
            }
            if (i == 0) { // 如果是第一行
                if (*origin.getPixel(i, j - 1) == 255) { // 如果左边的像素是白色
                    table.add(i, j, table.find(i, j - 1)); // 添加到等价表, 并与左边的像素合并
                    continue;
                }
                table.add(i, j); // 添加到等价表
                continue;
            }
            if (j == 0) { // 如果是第一列
                BYTE top = *origin.getPixel(i - 1, j); // 获取上面的像素值
                BYTE topRight = *origin.getPixel(i - 1, j + 1); // 获取右上角的像素值
                int topLabel = top == 255 ? table.find(i - 1, j) : -1; // 获取上面的像素的标签
                int topRightLabel = topRight == 255 ? table.find(i - 1, j + 1) : -1; // 获取右上角的像素的标签
                if (topLabel > 0 && topRightLabel > 0) { // 如果上面的像素和右上角的像素都是白色
                    table.add(i, j, table.merge(topLabel, topRightLabel)); // 添加到等价表, 并与上面的像素和右上角的像素合并
                    continue;
                }
                if (topLabel > 0) { // 如果上面的像素是白色
                    table.add(i, j, topLabel); // 添加到等价表, 并与上面的像素合并
                    continue;
                }
                table.add(i, j); // 添加到等价表
            }
            BYTE left = *origin.getPixel(i, j - 1); // 获取左边的像素值
            BYTE topLeft = *origin.getPixel(i - 1, j - 1); // 获取左上角的像素值
            BYTE top = *origin.getPixel(i - 1, j); // 获取上面的像素值
            BYTE topRight = *origin.getPixel(i - 1, j + 1); // 获取右上角的像素值
            int leftLabel = left == 255 ? table.find(i, j - 1) : -1; // 获取左边的像素的标签
            int topLeftLabel = topLeft == 255 ? table.find(i - 1, j - 1) : -1; // 获取左上角的像素的标签
            int topLabel = top == 255 ? table.find(i - 1, j) : -1; // 获取上面的像素的标签
            int topRightLabel = topRight == 255 ? table.find(i - 1, j + 1) : -1; // 获取右上角的像素的标签
            std::vector<int> labels; // 标签数组
            if (leftLabel > 0) // 如果左边的像素是白色
                labels.push_back(leftLabel); // 将左边的像素的标签加入标签数组
            if (topLeftLabel > 0) // 如果左上角的像素是白色
                labels.push_back(topLeftLabel); // 将左上角的像素的标签加入标签数组
            if (topLabel > 0) // 如果上面的像素是白色
                labels.push_back(topLabel); // 将上面的像素的标签加入标签数组
            if (topRightLabel > 0) // 如果右上角的像素是白色
                labels.push_back(topRightLabel); // 将右上角的像素的标签加入标签数组
            if (labels.size() == 0) { // 如果标签数组为空
                table.add(i, j); // 添加到等价表
                continue;
            }
            if (labels.size() == 1) { // 如果标签数组只有一个元素
                table.add(i, j, labels[0]); // 添加到等价表, 并与标签数组中的元素合并
                continue;
            }
            // 如果标签数组有多个元素
            int count = labels[0]; // 获取第一个元素
            for (int k = 1; k < labels.size(); k++) // 将标签数组中的元素两两合并
                count = table.merge(count, labels[k]);
            table.add(i, j, count); // 添加到等价表, 并与合并后的元素合并
        }
    // 获取等价表中的所有标签
    std::vector<int> labels;
    std::unordered_map<int, std::vector<Point>> t = table.getTable();
    for (auto it = t.begin(); it != t.end(); it++)
        labels.push_back(it->first);
    result = origin.grayToColor();
    // 遍历标签数组
    for (int i = 0; i < labels.size(); i++) {
        // 获取标签
        int label = labels[i];
        // 获取标签对应的所有点
        std::vector<Point> points = table.getPoints(label);
        BYTE *color = region_color::color[i % 7];
        // 遍历所有点
        for (int j = 0; j < points.size(); j++)
            // 绘制点
            result.setPixel(points[j].x, points[j].y, color);
    }
    return *this;
}

BMP RegionLabel::getResult() {
    return result;
}