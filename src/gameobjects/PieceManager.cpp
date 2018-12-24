#include "PieceManager.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cmath>
#include "constants.h"

const float PieceManager::GENERATE_DECREASE_FACTOR = 0.98f;
const float PieceManager::GENERATE_INITIAL_INTERVAL = 1500.0f;
const float PieceManager::GENERATE_MIN_INTERVAL = 500.0f;
const float PieceManager::SPEED = 1200.0f;

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
        m_generateEveryMS = std::max(m_generateEveryMS, GENERATE_MIN_INTERVAL);
    }

    return piece;
}

void PieceManager::movePieces(std::vector<std::unique_ptr<Piece>>& pieces, float deltaMS) const
{
    std::cout << "num of pieces " << pieces.size() << "\n";
    float speed = SPEED * (deltaMS/1000);

    if (speed > 100000.0f || std::isnan(speed)) return; // sometimes (e.g first frame) this value can be too high, ignore it
    /* Iterates through the pieces moving them and removing the elements whose z is less than -200.0f */
    for (auto it = pieces.begin(); it != pieces.end(); ) {
        Point3 curr = (*it)->getWorldPosition();
        curr = curr + ((m_rotationMatrix * Point3{0.0f, 0.0f, -1.0f}) * speed);
        (*it)->setPosition(curr);
        if (curr.z <= -200.0f)
            it = pieces.erase(it); // Renderable objects remove themselves from the scene when they are destroyed
        else
            ++it;
    }
}

PieceManager::~PieceManager()
{

}

