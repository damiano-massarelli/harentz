#ifndef PIECEGENERATOR_H
#define PIECEGENERATOR_H
#include <vector>
#include <memory>
#include <array>
#include "Piece.h"
#include "Renderer.h"
#include "Point3.h"
#include "Mat4.h"
#include <string>

/** \brief creates and moves pieces */
class PieceManager
{
    private:
        static const float GENERATE_INITIAL_INTERVAL; ///< Initial piece generation interval
        static const float GENERATE_MIN_INTERVAL; ///< min piece generation interval
        static const float GENERATE_DECREASE_FACTOR; ///< generation interval decrease factor
        static const float SPEED; ///< Speed of pieces, pixels per second

        float m_generateEveryMS = GENERATE_INITIAL_INTERVAL; ///< how frequently a new piece is generated
        float m_elapsedFromLast = 0.0f; ///< time elapsed from the last generated piece

        Renderer* m_renderer = nullptr; ///< the renderer used by the pieces
        const Point3 m_spawnPoint;
        const Mat4 m_rotationMatrix;

    public:
        // Lists all the available pieces
        static const std::vector<std::string> m_pieceNames;

        PieceManager(Renderer* renderer, const Point3& spawnPoint, const Mat4& rotationMatrix);

        /** \brief generates and returns a new piece
          * \param deltaMS ms elapsed since last frame
          * \return unique ptr to created piece or nullptr if a piece was not created. */
        std::unique_ptr<Piece> generatePiece(float deltaMS);

        /** \brief moves the pieces using the current speed */
        void movePieces(std::vector<std::unique_ptr<Piece>>& pieces, float deltaMS) const;

        virtual ~PieceManager();


};

#endif // PIECEGENERATOR_H
