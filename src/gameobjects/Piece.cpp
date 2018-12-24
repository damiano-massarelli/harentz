#include "Piece.h"
#include <sstream>
#include <string>
#include "DisplayManager.h"
#include "constants.h"
#include <functional>
#include <LinearTransition.h>
#include "ioUtils.h"

std::shared_ptr<Shape> Piece::CUBE_SHAPE;

template<typename T>
std::vector<T> split(const std::string &toSplit, char ch, std::function<T(std::string& str)> converter)
{
    std::stringstream ss(toSplit);
    std::vector<T> result;

    while( ss.good() )
    {
        std::string substr;
        getline(ss, substr, ch);
        result.push_back(converter(substr));
    }

    return result;
}

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

    float z = 0.0f;
    float sideSize = Piece::getCubeSide();

    std::stringstream infile = readFile("resources/pieces/" + shape + ".piece");
    std::string line;
    while(std::getline(infile, line)) {
        float x = 0.0f;
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
            for (; y < height; y++) {
                Transform* cube = new Transform{Piece::getCubeShape()};
                cube->setPosition(Point3{x, (-y * sideSize) - sideSize/2, z}); // -y: positive value in the file means the cube is higher
                addChild(cube);

                m_cubes.push_back(std::unique_ptr<Transform>(cube));
            }
            x += sideSize;
        }
        z -= sideSize; // comes towards the player
    }
    SDL_Color finalColor = readPieceColor(shape);
    setFillColor(finalColor);
    // Changes the opacity of this object, from 0 to what is specified in finalColor
    LinearTransition<int>::create(0, static_cast<int>(finalColor.a),
                             [this](int alpha) {
                                SDL_Color current = this->getFillColor();
                                current.a = alpha;
                                this->setFillColor(current);
                            },200.0f, nullptr, "game");
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

int Piece::getNumOfHorizontalCubes() const
{
    return m_horizontalCubes;
}

Piece::~Piece()
{
    //dtor
}
