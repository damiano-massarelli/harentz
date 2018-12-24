#include "BspRenderer.h"
#include "Face.h"
#include "drawers.h"
#include "SDL2/SDL.h"
#include <vector>
#include "BSPTree.h"

BspRenderer::BspRenderer(GPU_Target* screen, int screenWidth, int screenHeight, float screenZ, float projectionPointZ)
    : Renderer{screen, screenWidth, screenHeight, screenZ, projectionPointZ}
{
    //ctor
}

void BspRenderer::renderFaces(std::vector<pface>& faces)
{
    BSPTree renderTree{faces};

    // Walks through the bsptree and projects the faces
    renderTree.walk(Point3{0, 0, getProjectionPointZ()}, [this](const Face* f) {
        addToBatch(f);
    });

    drawBatch();
}


BspRenderer::~BspRenderer()
{
    //dtor
}
