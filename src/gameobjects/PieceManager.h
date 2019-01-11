#ifndef PIECEGENERATOR_H
#define PIECEGENERATOR_H
#include <vector>
#include <memory>
#include <array>
#include "Piece.h"
#include "Player.h"
#include "Renderer.h"
#include "Point3.h"
#include "Mat4.h"
#include <string>

class GameScene;

/** \brief creates and moves pieces */
class PieceManager
{
    private:
        static const float GENERATE_INITIAL_INTERVAL; ///< Initial piece generation interval
        static const float GENERATE_MIN_INTERVAL; ///< min piece generation interval
        static const float INITIAL_SPEED; ///< initial speed of pieces, pixels per second
        static const float FINAL_SPEED; ///< final speed for pieces, pixels per second
        static const int DEFAULT_FINAL_SCORE; ///< the score at which speed = FINAL_SPPED and a piece is generated every GENERATE_MIN_INTERVAL (can be changed, \sa m_scoreFinalSpeed)

        int m_finalScore = DEFAULT_FINAL_SCORE; ///< the score at which speed = FINAL_SPPED

        float m_elapsedFromLast = 0.0f; ///< time elapsed from the last generated piece

        Renderer* m_renderer = nullptr; ///< the renderer used by the pieces
        const Point3 m_spawnPoint;
        const Mat4 m_rotationMatrix;

        std::vector<std::unique_ptr<Piece>> m_pieces;

        /** \brief generates a new piece if enough time is elapsed
          * \param deltaMS ms elapsed since last frame */
        void generatePiece(float deltaMS, GameScene* gameScene);

        /** \brief generates a bonus/malus under a piece if it is has holes */
        void generateBonusMalus(const Piece* piece, int pieceLane, GameScene* gameScene);

        /** \brief checks collision between a piece and the player */
        void checkCollision(Piece* piece, Player* player);

    public:
        // Lists all the available pieces
        static const std::vector<std::string> m_pieceNames;

        PieceManager(Renderer* renderer, const Point3& spawnPoint, const Mat4& rotationMatrix);

        /** \brief moves the pieces using the current speed and creates new ones if necessary */
        void update(float deltaMS, GameScene* gameScene);

        /** \brief returns a vector of the currently used pieces */
        std::vector<std::unique_ptr<Piece>>& getPieces();

        virtual ~PieceManager();
};

#endif // PIECEGENERATOR_H
