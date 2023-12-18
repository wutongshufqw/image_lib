#ifndef INCLUDE_H
#define INCLUDE_H

#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

#if defined(_WIN32) || defined(_WIN64)
    #include <conio.h>
    #include <direct.h>
#elif defined(__linux__)
    #include <termios.h>
    #include <unistd.h>
#endif

#define EPS 1e-6
#define INT_INF 1e9
#define FLOAT_INF 1e9
#define DOUBLE_INF 1e10
#define PI acos(-1)

#endif