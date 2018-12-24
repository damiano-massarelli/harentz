#include "PaintersRenderer.h"
#include <algorithm>

PaintersRenderer::PaintersRenderer(GPU_Target* screen, int screenWidth, int screenHeight, float screenZ, float projectionPointZ)
    : Renderer{screen, screenWidth, screenHeight, screenZ, projectionPointZ}
{
    //ctor
}

void PaintersRenderer::renderFaces(std::vector<pface>& faces)
{
    // Sorts the faces according to their z values. Greater value means the face must be rendered later
    std::sort(faces.begin(), faces.end(),
            [](const auto& f1, const auto& f2) {
                return f1->getCenter().z > f2->getCenter().z;
            }
    );

    for (const auto& face : faces)
        addToBatch(face.get());

    drawBatch();
}


PaintersRenderer::~PaintersRenderer()
{
    //dtor
}
