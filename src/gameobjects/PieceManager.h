#ifndef PIECEGENERATOR_H
#define PIECEGENERATOR_H
#include <vector>
#include <memory>
#include <array>
#include <string>
#include "Piece.h"
#include "Player.h"
#include "Renderer.h"
#include "Point3.h"
#include "Bullet.h"
#include "Mat4.h"

class GameScene;

/** \brief creates and moves pieces */
class PieceManager
{
    private:
        static const std::vector<float> GENERATE_TIME_INTERVALS;
        static const std::vector<float> SPEED_INTERVALS;
        static const std::vector<float> TIME_INTERVALS;
        static const float BULLET_SPEED; ///< speed of the bullets

        float m_speedMultiplier = 1.0f;

        float m_elapsedFromLast = 0.0f; ///< time elapsed from the last generated piece
        float m_totalElapsed = 0.0f; ///< the total elapsed time used to determine the current speed and generation time.

        GameScene* m_gameScene = nullptr;
        Renderer* m_renderer = nullptr; ///< the renderer used by the pieces
        const Point3 m_spawnPoint;
        const Mat4 m_rotationMatrix;
        bool m_wireframeOnly = false; ///< should the new generated pieces be in wireframe mode?

        std::vector<std::unique_ptr<Piece>> m_pieces;
        std::vector<std::unique_ptr<Bullet>> m_bullets;

        /** \brief generates a new piece if enough time is elapsed
          * \param deltaMS ms elapsed since last frame */
        void generatePiece(float deltaMS);

        /** \brief generates a bonus/malus under a piece if it is has holes */
        void generateBonusMalus(const Piece* piece, int pieceLane);

        /** \brief checks collision between a piece and the player */
        void checkCollision(Piece* piece, Player* player);

    public:
        // Lists all the available pieces
        static const std::vector<std::string> m_pieceNames;

        PieceManager(GameScene* gameScene, Renderer* renderer, const Point3& spawnPoint, const Mat4& rotationMatrix);

        /** \brief moves the pieces using the current speed and creates new ones if necessary */
        void update(float deltaMS);

        /** \brief multiplies the current speed by a factor
          * \param factor the factor to use */
        void changeSpeed(float factor);

        /** \brief returns a vector of the currently used pieces */
        std::vector<std::unique_ptr<Piece>>& getPieces();

        void shootBullet();

        /** \brief if set to true the new generated pieces will be in wireframe mode */
        void setWireframeOnly(bool wireframeOnly);

        virtual ~PieceManager();
};

#endif // PIECEGENERATOR_H
