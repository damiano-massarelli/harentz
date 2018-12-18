#ifndef RENDERABLE_H
#define RENDERABLE_H

// Forward declaration
class Scene;

/** \brief An Abstract Class representing all the elements that can be added to a scene and rendered.
  * Renderable objects must implement render method in which they render their content using
  * a renderer. */
class AbstractRenderable
{
    private:
        Scene* m_scene = nullptr; ///< The scene to which this renderable belongs


    public:
        AbstractRenderable();

        void setScene(Scene* scene);

        virtual void render() = 0;

        void removeFromScene();

        virtual ~AbstractRenderable();

};

#endif // RENDERABLE_H
