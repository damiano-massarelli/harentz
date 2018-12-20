#include "randomUtils.h"
#include <cstdlib>

float randRange(float start, float end) {
    float f = static_cast<float>(std::rand())/(RAND_MAX + 1u);
    return start + (end - start)*f;
}
