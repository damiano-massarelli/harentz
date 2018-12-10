#ifndef PIECEGENERATOR_H
#define PIECEGENERATOR_H
#include <vector>
#include <memory>
#include "Piece.h"
#include "Renderer.h"

/** \brief creates and moves pieces */
class PieceManager
{
    private:
        static constexpr float INITIAL_INTERVAL = 2000.0f;
        static constexpr float MIN_INTERVAL = 500.0f;
        static constexpr float DECREASE_FACTOR = 0.97;

        float m_everyMS = INITIAL_INTERVAL; ///< how frequently a new piece is generated
        float m_elapsedFromLast = 0.0f; ///< time elapsed from the last generated piece

        Renderer* m_renderer = nullptr; ///< the renderer used by the pieces

    public:
        PieceManager(Renderer* renderer);

        /** \brief generates and returns a new piece
          * \param deltaMS ms elapsed since last frame
          * \return unique ptr to created piece or nullptr if a piece was not created. */
        std::unique_ptr<Piece> generatePiece(float deltaMS);

        /** \brief moves the pieces using the current speed */
        void movePieces(const std::vector<std::unique_ptr<Piece>>& pieces) const;

        virtual ~PieceManager();


};

#endif // PIECEGENERATOR_H
