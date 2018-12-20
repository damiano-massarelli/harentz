#ifndef PAINTERSRENDERER_H
#define PAINTERSRENDERER_H
#include "Renderer.h"
#include <vector>
#include "SDL2/SDL.h"

class PaintersRenderer : public Renderer
{
    public:
        PaintersRenderer(SDL_Renderer* renderer, int screenWidth, int screenHeight, float screenZ, float projectionPointZ);

        virtual void renderFaces(std::vector<pface>& faces) override;

        virtual ~PaintersRenderer();

};

#endif // PAINTERSRENDERER_H
