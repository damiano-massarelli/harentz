#ifndef MEMORYUTILS_H
#define MEMORYUTILS_H
#include <memory>
#include <SDL_gpu.h>

struct ImageDeleter {
    void operator()(GPU_Image* img);
};
using imageUniquePtr = std::unique_ptr<GPU_Image, ImageDeleter>;

#endif // MEMORYUTILS_H
