#include "PieceManager.h"
#include "constants.h"
#include "GameScene.h"
#include "collisionDetection.h"
#include "BonusPiece.h"
#include "MalusPiece.h"
#include "Bullet.h"
#include "constants.h"
#include "randomUtils.h"
#include "lerpUtils.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cmath>

const std::vector<float> PieceManager::GENERATE_TIME_INTERVALS{900.0f, 500.0f, 500.0f, 350.0f};
const std::vector<float> PieceManager::SPEED_INTERVALS{1500.0f, 3500.0f, 3500.0f, 5100.0f};
const std::vector<float> PieceManager::TIME_INTERVALS{0.0f, 25 * 1000.0f, 55 * 1000.0f, 105 * 1000.f};
const float PieceManager::BULLET_SPEED = 2000.0f;
const std::vector<std::string> PieceManager::m_pieceNames{"I-side", "I-up", "J", "J-up", "L", "L-up", "O-side", "O-up",
                                                    "S", "T-down", "T-up", "Z", "J-down", "L-down"};

bool checkPieceCollision(const Piece* p1, const Piece* p2, int offset) {
    for (int i = 0; i < p1->getNumOfHorizontalCubes(); ++i)
        for (int j = 0; j < p1->getNumOfVericalCubes(); ++j)
            if (p1->cubeAt(i, j, 0) && p2->cubeAt(i - offset, j, 0))
                return true;

    return false;
}

PieceManager::PieceManager(GameScene* gameScene, Renderer* renderer, const Point3& spawnPoint, const Mat4& rotationMatrix) :
                    m_gameScene{gameScene}, m_renderer{renderer}, m_spawnPoint{spawnPoint}, m_rotationMatrix{rotationMatrix}
{
    std::srand(std::time(nullptr)); // initializes random values
}

void PieceManager::generatePiece(float deltaMS)
{
    m_elapsedFromLast += deltaMS;
    float generationInterval = lerpIntervals(TIME_INTERVALS, GENERATE_TIME_INTERVALS, m_totalElapsed) / m_speedMultiplier;

    std::unique_ptr<Piece> piece;
    std::unique_ptr<Piece> piece2;
    if (m_elapsedFromLast > generationInterval) {
        int pieceIndex = randRangeInt(0, m_pieceNames.size());
        piece = std::make_unique<Piece>(m_renderer, m_pieceNames[pieceIndex]);

        // Selects a lane for the piece
        int lane = randRangeInt(0, NUMBER_OF_LANES - piece->getNumOfHorizontalCubes() + 1);

        bool secondPieceGenerated = false;

        if (m_totalElapsed > TIME_INTERVALS[3]) {
            int piece2Index = randRangeInt(0, m_pieceNames.size());
            piece2 = std::make_unique<Piece>(m_renderer, m_pieceNames[piece2Index]);

            // Selects a lane for the second piece
            int lane2 = randRangeInt(0, NUMBER_OF_LANES - piece2->getNumOfHorizontalCubes() + 1);

            if (!checkPieceCollision(piece.get(), piece2.get(), lane2 - lane)) {
                secondPieceGenerated = true;
                piece2->setPosition(Point3{piece2->xForLane(lane2), m_spawnPoint.y, m_spawnPoint.z});
                piece2->setTransformationMatrix(m_rotationMatrix);
                if (m_wireframeOnly) // wireframe mode is on
                    piece2->setFillColor(SDL_Color{0, 0, 0, 0});

                // Adds the piece to the scene
                m_gameScene->add(piece2.get());

                // Adds the piece to the list
                m_pieces.push_back(std::move(piece2));
            }
        }

        // Place and rotate the piece
        piece->setPosition(Point3{piece->xForLane(lane), m_spawnPoint.y, m_spawnPoint.z});
        piece->setTransformationMatrix(m_rotationMatrix);
        if (m_wireframeOnly) // wireframe mode is on
            piece->setFillColor(SDL_Color{0, 0, 0, 0});

        /* could generates a bonus or malus under the current piece */
        if (!secondPieceGenerated)
            generateBonusMalus(piece.get(), lane);

        m_elapsedFromLast = 0.0f; // reset elapsed time

        // Adds the piece to the scene
        m_gameScene->add(piece.get());

        // Adds the piece to the list
        m_pieces.push_back(std::move(piece));
    }
}

void PieceManager::generateBonusMalus(const Piece* piece, int pieceLane)
{
    for (int i = 0; i < piece->getNumOfHorizontalCubes(); ++i) {
        for (int j = 0; j < std::min(piece->getNumOfVericalCubes(), 2); ++j) {
            if (piece->cubeAt(i, j, 0) == false && randRange(0.0f, 1.0f) <= 0.15f) { // checks if the piece is hollow at level j and horizontal position i
                std::unique_ptr<Piece> bonusMalus;

                // bonus pieces are more likely than malus pieces
                if (randRange(0.0f, 1.0f) >= 0.6f)
                    bonusMalus = std::make_unique<BonusPiece>(m_renderer);
                else
                    bonusMalus = std::make_unique<MalusPiece>(m_renderer);

                bonusMalus->setPosition(Point3{bonusMalus->xForLane(pieceLane + i),
                                         m_spawnPoint.y,
                                         m_spawnPoint.z} + m_rotationMatrix * Point3{0.0f, -Piece::getCubeSide()*j - Piece::getCubeSide()/4});
                bonusMalus->setTransformationMatrix(m_rotationMatrix);
                if (m_wireframeOnly)
                    bonusMalus->setFillColor(SDL_Color{0, 0, 0, 0});

                m_gameScene->add(bonusMalus.get());
                m_pieces.push_back(std::move(bonusMalus));
            }
        }
    }
}

void PieceManager::shootBullet()
{
    auto bullet = std::make_unique<Bullet>(m_renderer);
    bullet->setPosition(m_gameScene->getPlayer()->getWorldPosition());
    bullet->setTransformationMatrix(m_rotationMatrix);

    m_gameScene->add(bullet.get());

    m_bullets.push_back(std::move(bullet));
}

void PieceManager::update(float deltaMS)
{
    m_totalElapsed += deltaMS; // updates total elapsed time

    /* Generate a new piece if necessary */
    generatePiece(deltaMS);

    float speed = lerpIntervals(TIME_INTERVALS, SPEED_INTERVALS, m_totalElapsed) * m_speedMultiplier;

    speed *= (deltaMS/1000); // speed is fps independent

    if (speed > 100000.0f || std::isnan(speed)) return; // sometimes (e.g first frame) this value can be too high, ignore it
    /* Iterates through the pieces moving them and removing the elements whose z is less than -200.0f */
    for (auto it = m_pieces.begin(); it != m_pieces.end(); ) {
        Point3 currPos = (*it)->getWorldPosition();
        Point3 nextPos = currPos + ((m_rotationMatrix * Point3{0.0f, 0.0f, -1.0f}) * speed);

        /* Adds score if the piece has gone behind the player
         * The score added is the result of SCORE_PER_PIECE multiplied by the
         * number of cubes composing the piece */
        float playerZ = m_gameScene->getPlayer()->getWorldPosition().z;
        if (currPos.z > playerZ && nextPos.z < playerZ)
            m_gameScene->incrementScore(SCORE_PER_PIECE * (*it)->getChildren().size());

        // Updates the position
        (*it)->setPosition(nextPos);

        // Checks collisions
        checkCollision((*it).get(), m_gameScene->getPlayer());

        // Removes pieces that are too far away
        if (nextPos.z <= -200.0f)
            it = m_pieces.erase(it); // Renderable objects remove themselves from the scene when they are destroyed
        else
            ++it;
    }

    // updates the bullets
    float bulletSpeed = BULLET_SPEED * (deltaMS/1000);
    for (auto it = m_bullets.begin(); it != m_bullets.end(); ) {
        Point3 currPos = (*it)->getWorldPosition();
        Point3 nextPos = currPos + ((m_rotationMatrix * Point3{0.0f, 0.0f, 1.0f}) * bulletSpeed);

        (*it)->setPosition(nextPos);
        if (nextPos.z > m_spawnPoint.z)
            it = m_bullets.erase(it);
        else
            ++it;
    }
}

void PieceManager::checkCollision(Piece* piece, Player* player)
{
    /* The collision must be checked with the cube contained in the player
     * its position is different from that of the player itself */
    const std::vector<Transform*>& children = piece->getChildren();
    const Transform* playerCube = (player->getChildren().size() == 0 ? nullptr : player->getChildren()[0]);

    for (int cubeIndex = children.size() - 1; cubeIndex >= 0; --cubeIndex) {
        /* Collision with player */
        if (sphereDetection(children[cubeIndex], playerCube, (Piece::getCubeSide() / 2) * 0.9f)) { // Collision found return the index
            piece->handleCollision(cubeIndex);
            break;
        }
        /* Collision with bullets */
        for (auto bulletIt = m_bullets.begin(); bulletIt != m_bullets.end();) {
            if (sphereDetection(children[cubeIndex], (*bulletIt)->getChildren()[0], (Piece::getCubeSide() / 2) * 0.5f)) {
                piece->explodeCube(cubeIndex);
                bulletIt = m_bullets.erase(bulletIt);
                m_gameScene->incrementScore(BULLET_HIT_MULTIPLIER * SCORE_PER_PIECE);
                break; // cannot check collision with already removed child
            } else
                ++bulletIt;
        }
    }
}

std::vector<std::unique_ptr<Piece>>& PieceManager::getPieces()
{
    return m_pieces;
}

void PieceManager::changeSpeed(float factor)
{
    m_speedMultiplier = factor;
}

void PieceManager::setWireframeOnly(bool wireframeOnly)
{
    m_wireframeOnly = wireframeOnly;
}

PieceManager::~PieceManager()
{

}
