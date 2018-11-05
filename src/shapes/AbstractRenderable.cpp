#include "AbstractRenderable.h"
#include "Scene.h"

AbstractRenderable::AbstractRenderable()
{

}

void AbstractRenderable::setScene(Scene* scene)
{
    m_scene = scene;
}

void AbstractRenderable::removeFromScene()
{
    if (m_scene != nullptr)
        m_scene->remove(this);
    m_scene = nullptr;
}

AbstractRenderable::~AbstractRenderable()
{
    removeFromScene();
}
