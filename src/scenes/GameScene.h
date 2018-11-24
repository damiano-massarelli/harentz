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


class GameScene : public Scene
{
    private:
        static const float PROJECTION_POINT_Z;
        static const float SCREEN_Z;

        std::unique_ptr<Renderer> m_3dRenderer;
        std::vector<std::unique_ptr<Piece>> m_pieces;
        std::unique_ptr<Ground> m_ground;

        Point3 m_spawnPoint; ///< the point where new pieces are spawned, only y and z are relevant.
        Mat4 m_rotationMatrix; ///< the matrix used by all the rotated objects

        int elapsedFrames = 0;

        /** \brief computes and stores in m_rotationMatrix the rotation matrix for rotated objects
          * this calculation is based on m_spawnPoint which specifies
          * where the end of the ground should be.
          *      spawn_pt
          *      /|
          *     / |      the angle t can be calculated as arcsin(spawn_pt.y / GROUND_DEPTH)
          *    /t |
          *   /)__| */
        void computeRotationMatrix();

    public:
        GameScene();

        virtual void onShow(SDL_Window* window, SDL_Renderer* renderer) override;

        virtual void onEvent(SDL_Event e) override;

        virtual void onRenderingComplete() override;

        virtual ~GameScene();

};

#endif // GAMESCENE_H
