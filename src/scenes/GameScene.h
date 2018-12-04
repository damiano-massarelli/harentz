#ifndef GAMESCENE_H
#define GAMESCENE_H
#include "Scene.h"
#include "Renderer.h"
#include "Piece.h"
#include <iostream>
#include <memory>
#include <vector>
#include "Ground.h"
#include "Point3.h"
#include "Mat4.h"
#include "Player.h"

class GameScene : public Scene
{
    private:
        /* Pieces and ground use two different renderers so that pieces are always drawn above the ground */
        std::unique_ptr<Renderer> m_3dRenderer;
        std::unique_ptr<Renderer> m_groundRenderer;
        std::vector<std::unique_ptr<Piece>> m_pieces;
        std::unique_ptr<Ground> m_ground;
        std::unique_ptr<Player> m_player;

        Point3 m_spawnPoint; ///< the point where new pieces are spawned, only y and z are relevant.
        Mat4 m_rotationMatrix; ///< the matrix used by all the rotated objects

        int elapsedFrames = 0;


    public:
        GameScene();

        virtual void onShow(SDL_Window* window, SDL_Renderer* renderer) override;

        virtual void onEvent(SDL_Event e) override;

        virtual void onRenderingComplete() override;

        virtual void onRemove() override;

        virtual ~GameScene();

};

#endif // GAMESCENE_H
