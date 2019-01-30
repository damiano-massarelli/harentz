#include "memoryUtils.h"

void ImageDeleter::operator()(GPU_Image* img)
{
    GPU_FreeImage(img);
}
