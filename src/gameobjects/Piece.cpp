#include "Piece.h"
#include "DisplayManager.h"
#include "constants.h"
#include "LinearTransition.h"
#include "ioUtils.h"
#include "GameScene.h"
#include "BreakingParticles.h"
#include "stringUtils.h"
#include <sstream>
#include <string>
#include <functional>
#include <algorithm>

std::shared_ptr<Shape> Piece::CUBE_SHAPE;

std::shared_ptr<Shape> Piece::getCubeShape()
{
    if (!CUBE_SHAPE) {
        float cubeSize = Piece::getCubeSide()/2;
        CUBE_SHAPE = std::shared_ptr<Shape>(
                     new Shape{std::vector<Point3>{ Point3{-cubeSize, -cubeSize, -cubeSize},
                     Point3{cubeSize, -cubeSize, -cubeSize},
                     Point3{cubeSize, cubeSize, -cubeSize},
                     Point3{-cubeSize, cubeSize, -cubeSize},
                     Point3{cubeSize, -cubeSize, cubeSize},
                     Point3{cubeSize, cubeSize, cubeSize},
                     Point3{-cubeSize, -cubeSize, cubeSize},
                     Point3{-cubeSize, cubeSize, cubeSize}},
                  std::vector<int>{
                         1, 4, 5, 2, // right face
                         0, 3, 7, 6, // left face
                         0, 1, 2, 3, // front face
                         7, 5, 4, 6, // back face
                         0, 6, 4, 1}}); // top face
    }

    return CUBE_SHAPE;
}

float Piece::getCubeSide()
{
    return (DisplayManager::screenWidth() / static_cast<float>(NUMBER_OF_LANES));
}

SDL_Color Piece::readPieceColor(const std::string& shape)
{
    // For names like T-down gets T
    std::string colorFileName = split<std::string>(shape, '-', [](std::string& str) { return str; })[0];

    std::stringstream colorString = readFile("resources/pieces/" + colorFileName + ".color");
    std::vector<Uint8> channels = split<Uint8>(colorString.str(), ',', [](std::string& str) { return static_cast<Uint8>(std::stoi(str)); } );
    return SDL_Color{channels[0], channels[1], channels[2], channels[3]};
}

Piece::Piece(Renderer* renderer, const std::string& shape) : Transform{nullptr}
{
    setRenderer(renderer);

    int z = 0;
    float sideSize = Piece::getCubeSide();

    std::stringstream infile = readFile("resources/pieces/" + shape + ".piece");
    std::string line;
    while(std::getline(infile, line)) {
        int x = 0;
        std::vector<int> cubesHeights = split<int>(line, ' ', [](std::string& str) { return std::stoi(str); });
        m_horizontalCubes = std::max(m_horizontalCubes, static_cast<int>(cubesHeights.size())); // updates the number of horizontal cubes

        for (int height : cubesHeights) {
            int y = 0;

            /* Negative values means that the cube should be placed at that
            height without filling the space below it */
            if (height < 0) {
                height = -height;
                y = height - 1;
            }
            m_verticalCubes = std::max(m_verticalCubes, height);

            for (; y < height; y++) {
                Transform* cube = new Transform{Piece::getCubeShape()};
                cube->setPosition(Point3{x * sideSize, (-y * sideSize) - sideSize/2, z * sideSize}); // -y: positive value in the file means the cube is higher
                addChild(cube);

                m_cubes.push_back(std::unique_ptr<Transform>(cube));

                m_cubeAtPosition.insert(std::make_tuple(x, y, z));
            }
            x += 1;
        }
        z -= 1; // comes towards the player
    }
    SDL_Color finalColor = readPieceColor(shape);
    setFillColor(finalColor);
    // Changes the opacity of this object, from 0 to what is specified in finalColor
    LinearTransition<int>::create(0, static_cast<int>(finalColor.a),
                             [this](int alpha) {
                                SDL_Color current = this->getFillColor();
                                current.a = alpha;
                                this->setFillColor(current);
                            }, 200.0f, nullptr, "game");
}

float Piece::xForLane(int lane) const
{
    return (lane * Piece::getCubeSide()) - DisplayManager::screenWidth()/2.0f + Piece::getCubeSide()/2;
}

void Piece::removeCube(int index)
{
    if (index < m_cubes.size()) { // Just in case
        removeChild(m_cubes[index].get()); // Removes it visually
        m_cubes.erase(m_cubes.begin() + index); // Free its memory
    }
}

void Piece::explodeCube(int index)
{
    GameScene* gameScene = static_cast<GameScene*>(DisplayManager::getInstance()->getCurrentScene()); // it must be the game scene

    /* Creates the particle effect */
    Point3 collisionPoint = getChildren()[index]->getWorldPosition();
    SDL_Color particleColor = getChildren()[index]->getFillColor();
    gameScene->add(BreakingParticles::create(gameScene->getEffectRenderer(), collisionPoint, particleColor, 10));

    /* finally removes the cube */
    removeCube(index);

}

int Piece::getNumOfHorizontalCubes() const
{
    return m_horizontalCubes;
}

int Piece::getNumOfVericalCubes() const
{
    return m_verticalCubes;
}

void Piece::handleCollision(int collidedCubeIndex)
{
    /* removes the cube that collided */
    explodeCube(collidedCubeIndex);

    /* Decreases the number of lives of the player */
    GameScene* gameScene = static_cast<GameScene*>(DisplayManager::getInstance()->getCurrentScene()); // it must be the game scene
    gameScene->incrementLives(-1);
}

bool Piece::cubeAt(int x, int y, int z) const
{
    return m_cubeAtPosition.find(std::make_tuple(x, y, z)) != m_cubeAtPosition.end();
}

Piece::~Piece()
{
    //dtor
}
