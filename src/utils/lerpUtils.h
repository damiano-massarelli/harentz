#ifndef LERPUTILS_H
#define LERPUTILS_H
#include <vector>
#include <stdexcept>

/** \brief linearly interpolates between v0 and v1 */
template<typename T>
T lerp(const T& v0, const T& v1, float t) {
    return v0 + (v1 - v0) * t;
}

template<typename I, typename V>
V lerpIntervals(const std::vector<I>& intervals, const std::vector<V>& values, I toLerp) {
    if (intervals.size() != values.size() || intervals.size() < 2)
        throw std::invalid_argument("intervals and values must have the same size (min is 2)");
    for (std::size_t i = 0; i < intervals.size() - 1; ++i) {
        if (toLerp >= intervals[i] && toLerp < intervals[i+1]) {
            float t = static_cast<float>((toLerp - intervals[i])/(intervals[i+1] - intervals[i]));
            return lerp(values[i], values[i+1], t);
        }
    }
    return *(values.end() - 1); // returns the last element
}

#endif // LERPUTILS_H
