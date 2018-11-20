#ifndef GAMESCENE_H
#define GAMESCENE_H
#include "Scene.h"
#include "Renderer.h"
#include "Piece.h"
#include <iostream>
#include <memory>
#include <vector>


class GameScene : public Scene
{
    private:
        std::unique_ptr<Renderer> m_3dRenderer;
        std::vector<std::unique_ptr<Piece>> m_pieces;

        int elapsedFrames = 0;

    public:
        GameScene();

        virtual void onShow(SDL_Window* window, SDL_Renderer* renderer) override;

        virtual void onEvent(SDL_Event e) override;

        virtual void onRenderingComplete() override;

        virtual ~GameScene();

};

#endif // GAMESCENE_H
