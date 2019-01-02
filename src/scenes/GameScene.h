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
#include "PieceManager.h"
#include "StarField.h"
#include <SDL_gpu.h>

class GameScene : public Scene
{
    private:
        /* Pieces and ground use two different renderers so that pieces are always drawn above the ground */
        std::unique_ptr<Renderer> m_3dRenderer;
        std::unique_ptr<Renderer> m_effectRenderer;
        std::vector<std::unique_ptr<Piece>> m_pieces;
        std::unique_ptr<Ground> m_ground;
        std::unique_ptr<Player> m_player;

        std::unique_ptr<PieceManager> m_pieceManager;

        std::unique_ptr<StarField> m_starFieldEffect;

        Point3 m_spawnPoint; ///< the point where new pieces are spawned, only y and z are relevant.
        Mat4 m_rotationMatrix; ///< the matrix used by all the rotated objects

    public:
        GameScene();

        virtual void onShow(GPU_Target* screen) override;

        virtual void onEvent(SDL_Event e) override;

        virtual void onRenderingComplete() override;

        virtual void onRemove() override;

        Renderer* getEffectRenderer();

        Player* getPlayer();

        virtual ~GameScene();

};

#endif // GAMESCENE_H
