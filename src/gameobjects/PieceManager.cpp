#include "PieceManager.h"
#include <cstdlib>
#include <ctime>
#include "constants.h"

const std::vector<std::string> PieceManager::m_pieceNames{"I-side", "I-up", "J", "J-up", "L", "L-up", "O-side", "O-up",
                                                    "S", "T-down", "T-up", "Z"};

PieceManager::PieceManager(Renderer* renderer, const Point3& spawnPoint, const Mat4& rotationMatrix) :
                    m_renderer{renderer}, m_spawnPoint{spawnPoint}, m_rotationMatrix{rotationMatrix}
{
    std::srand(std::time(nullptr)); // initializes random values
}

std::unique_ptr<Piece> PieceManager::generatePiece(float deltaMS)
{
    m_elapsedFromLast += deltaMS;
    std::unique_ptr<Piece> piece;
    if (m_elapsedFromLast > m_generateEveryMS) {
        int pieceIndex = (std::rand() / ((RAND_MAX + 1u)/m_pieceNames.size()));
        piece = std::make_unique<Piece>(m_renderer, m_pieceNames[pieceIndex]);

        // Selects a lane for the piece
        int lane = (std::rand() / ((RAND_MAX + 1u)/(NUMBER_OF_LANES - piece->getNumOfHorizontalCubes() + 1)));

        // Place and rotate the piece
        piece->setPosition(Point3{piece->xForLane(lane), m_spawnPoint.y, m_spawnPoint.z});
        piece->setTransformationMatrix(m_rotationMatrix);

        m_elapsedFromLast = 0.0f; // reset elapsed time
        m_generateEveryMS *= GENERATE_DECREASE_FACTOR;
        float max_ = GENERATE_MIN_INTERVAL;
        std::cout << max_ << "\n";
        m_generateEveryMS = std::max(m_generateEveryMS, max_);
    }

    return piece;
}

void PieceManager::movePieces(const std::vector<std::unique_ptr<Piece>>& pieces, float deltaMS) const
{

}

PieceManager::~PieceManager()
{

}

