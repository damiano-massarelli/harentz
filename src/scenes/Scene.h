#ifndef ABSTRACTSCENE_H
#define ABSTRACTSCENE_H
#include <vector>
#include "EventListener.h"
#include "EventListenerCrumb.h"

class AbstractRenderable;

/** \brief An class performing all the basic operations of a scene */
class Scene : public EventListener
{
    private:
        std::vector<AbstractRenderable* > m_renderList;
        std::unique_ptr<EventListenerCrumb> m_eventCrumb;

        SDL_Color m_bgColor{0, 0, 0};

        SDL_Renderer* m_sdlRenderer = nullptr;


    public:
        Scene();

        /** \brief reacts to enter frame events
          * each time a new frame is created this method
          * called so that Renderable objects can be actually
          * on screen.
          */
        virtual void onEvent(SDL_Event e) override;

        /** \brief this function is called when all the element of the scene have been rendered
          * This function is sometimes required by those renderer that draws on the screen once the
          * all their elements have been drawn */
        virtual void onRenderingComplete() {};

        /** \brief method called when the scene is passed to the DisplayManager.
          * This method is called by the DisplayManager to pass the main SDL_Renderer
          * so that the Scene can create its own renderers using it. */
        virtual void onShow(SDL_Window* window, SDL_Renderer* renderer);

        /** \brief adds a new object to the scene */
        void add(AbstractRenderable* renderable);

        /** \brief removes an object from the scene */
        void remove(AbstractRenderable* renderable);

        /** \brief set the scene's background color */
        void setBgColor(const SDL_Color& color);

        virtual ~Scene();

};

#endif // ABSTRACTSCENE_H
