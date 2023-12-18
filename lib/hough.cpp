#include "include/hough.h"

namespace hough {
    // 赤橙黄绿青蓝紫的RGB值
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

Hough::Hough(BMP& origin) {
    if (origin.getBitCount() != 8)
        origin = origin.grayScale();
    this->origin = origin;
}

Hough Hough::lineDetect(double rate) {
    int width = origin.getWidth();
    int height = origin.getHeight();
    // 建立hash表
    std::unordered_map<int, std::unordered_map<int, int>> hash;
    // 计算r的取值范围
    int rMax = (int) sqrt((width * width + height * height) / 2);
    int sum = 0;
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height;j++)
            if (*origin.getPixel(i, j) < 128) {
                sum++;
                for (int k = -90; k < 90; k++) {
                    // 通过极坐标方程计算出对应的r值(通过x10以保留小数点后一位)
                    int r = (int)((i * cos(k * PI / 180) + j * sin(k * PI / 180)) * 10);
                    if (r < -rMax * 10 || r > rMax * 10)
                        continue;
                    // 通过hash表记录每个r值对应的点数
                    // 如果该r值已经存在，则将对应的点数加一
                    // 否则，将该r值加入hash表，并将对应的点数置为1
                    if (hash.find(r) != hash.end()) {
                        if (hash[r].find(k) != hash[r].end())
                            hash[r][k]++;
                        else
                            hash[r][k] = 1;
                    }
                    else {
                        std::unordered_map<int, int> temp;
                        temp[k] = 1;
                        hash[r] = temp;
                    }
                    
                }
            }
    int max = 0;
    // 确定阈值：0.5 * max
    for (auto it = hash.begin(); it != hash.end(); it++)
        for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++)
            if (it2->second > max)
                max = it2->second;
    int threshold = (int) max * rate;
    // 将满足阈值的点加入到result中
    std::vector<std::pair<int, int>> points;
    for (auto it = hash.begin(); it != hash.end(); it++)
        for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++)
            if (it2->second > threshold)
                points.push_back(std::make_pair(it->first, it2->first));
    // 将满足阈值的点按照点数从大到小排序
    std::sort(points.begin(), points.end(), [](std::pair<int, int> a, std::pair<int, int> b) {
        return a.second > b.second;
    });
    // 将满足阈值的点画出来
    result = origin.grayToColor();
    for (int i = 0; i < points.size(); i++) {
        int r = points[i].first;
        int theta = points[i].second;
        result.drawStraightLine(r, theta, 255, 0, 0);
    }
    return *this;
}

BMP Hough::getResult() {
    return result;
}