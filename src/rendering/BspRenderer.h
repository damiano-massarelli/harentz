#ifndef BSPRENDERER_H
#define BSPRENDERER_H
#include "Renderer.h"
#include <vector>
#include <SDL_gpu.h>

/** \brief a renderer that uses bsp trees to sort faces */
class BspRenderer : public Renderer
{
    public:
        BspRenderer(GPU_Target* screen, int screenWidth, int screenHeight, float screenZ, float projectionPointZ);

        virtual void renderFaces(std::vector<pface>& faces) override;

        virtual ~BspRenderer();

};

#endif // BSPRENDERER_H
