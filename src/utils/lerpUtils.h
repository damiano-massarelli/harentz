#ifndef LERPUTILS_H
#define LERPUTILS_H

/** \brief linearly interpolates between v0 and v1 */
template<typename T>
T lerp(const T& v0, const T& v1, float t) {
    return v0 + (v1 - v0) * t;
}

#endif // LERPUTILS_H
