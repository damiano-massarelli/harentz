#ifndef ABSTRACTSCENE_H
#define ABSTRACTSCENE_H
#include <vector>
#include <memory>
#include "EventListener.h"
#include "EventListenerCrumb.h"
#include <SDL_gpu.h>

class AbstractRenderable;

/** \brief An class performing all the basic operations of a scene */
class Scene : public EventListener
{
    private:
        std::vector<AbstractRenderable* > m_renderList;
        std::unique_ptr<EventListenerCrumb> m_enterFrameCrumb;
        std::unique_ptr<EventListenerCrumb> m_willEnterBgCrumb;
        std::unique_ptr<EventListenerCrumb> m_didEnterBgCrumb;
        std::unique_ptr<EventListenerCrumb> m_willEnterFgCrumb;
        std::unique_ptr<EventListenerCrumb> m_didEnterFgCrumb;

        SDL_Color m_bgColor{0, 0, 0, 255};

        GPU_Target* m_screen = nullptr;

    protected:
        /** \brief Describes status of a resume/pause event */
        enum class EventStatus {
            WILL,
            DID
        };

    public:
        Scene();

        /** \brief reacts to enter frame events
          * each time a new frame is created this method
          * called so that Renderable objects can be actually
          * on screen.
          */
        virtual void onEvent(SDL_Event event) override;

        virtual void onEnterFrame(SDL_Event& e);

        /** \brief this function is called when all the element of the scene have been rendered
          * This function is sometimes required by those renderer that draws on the screen once
          * all their elements have been rendered (that is render is called on them) */
        virtual void onRenderingComplete() {};

        /** \brief this function is called whenever the application is paused */
        virtual void onPause(const EventStatus& status) {}

        /** \brief this method is called whenever the application is resumed */
        virtual void onResume(const EventStatus& status) {}

        /** \brief this function is called before the scene is removed */
        virtual void onRemove() {};

        /** \brief method called when the scene is passed to the DisplayManager.
          * This method is called by the DisplayManager to pass the main screen
          * so that the Scene can create its own renderers using it. */
        virtual void onShow(GPU_Target* screen);

        /** \brief adds a new object to the scene */
        void add(AbstractRenderable* renderable);

        /** \brief removes an object from the scene */
        void remove(AbstractRenderable* renderable);

        /** \brief set the scene's background color */
        void setBgColor(const SDL_Color& color);

        /** \brief returns the current screen */
        GPU_Target* getScreen();

        virtual ~Scene();

};

#endif // ABSTRACTSCENE_H
