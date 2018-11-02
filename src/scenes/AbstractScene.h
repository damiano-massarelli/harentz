#ifndef ABSTRACTSCENE_H
#define ABSTRACTSCENE_H
#include <vector>
#include "EventListener.h"
#include "EventListenerCrumb.h"
#include "Renderable.h"

/** \brief An abstract class performing all the basic operations of a scene */
class AbstractScene : public EventListener
{
    private:
        std::vector<Renderable* > m_renderList;
        std::unique_ptr<EventListenerCrumb> m_eventCrumb;

    public:
        AbstractScene();

        /** \brief reacts to enter frame events
          * each time a new frame is created this method
          * called so that Renderable objects can be actually
          * on screen.
          */
        virtual void onEvent(SDL_Event e) override;

        /** \brief method called when the scene is passed to the DisplayManager.
          * This method is called by the DisplayManager to pass the main SDL_Renderer
          * so that the Scene can create its own renderers using it. */
        virtual void onCreate(SDL_Renderer* renderer) = 0;

        /** \brief adds a new object to the scene */
        void add(Renderable* renderable);

        /** \brief removes an object from the scene */
        void remove(Renderable* renderable);

        virtual ~AbstractScene();

};

#endif // ABSTRACTSCENE_H
