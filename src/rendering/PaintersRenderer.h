#ifndef PAINTERSRENDERER_H
#define PAINTERSRENDERER_H
#include "Renderer.h"
#include <vector>
#include <SDL_gpu.h>

class PaintersRenderer : public Renderer
{
    public:
        PaintersRenderer(GPU_Target* screen, int screenWidth, int screenHeight, float screenZ, float projectionPointZ);

        virtual void renderFaces(std::vector<pface>& faces) override;

        virtual ~PaintersRenderer();

};

#endif // PAINTERSRENDERER_H
