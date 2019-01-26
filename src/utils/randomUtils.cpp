#include "randomUtils.h"
#include <random>

std::random_device rd;
std::mt19937 mt(rd());

float randRange(float start, float end) {
    std::uniform_real_distribution<float> dist(start, end);
    return dist(mt);
}

int randRangeInt(int start, int end)
{
    std::uniform_int_distribution<int> dist(start, end - 1);
    return dist(mt);
}
