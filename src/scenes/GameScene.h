#ifndef GAMESCENE_H
#define GAMESCENE_H
#include "AbstractScene.h"
#include <iostream>


class GameScene : public AbstractScene
{
    public:
        GameScene();

        virtual void onCreate(SDL_Renderer* renderer) override {
            // do something
        }

        virtual void onEvent(SDL_Event e) override {
            // do something, maybe
        }

        virtual ~GameScene();

    private:
};

#endif // GAMESCENE_H
