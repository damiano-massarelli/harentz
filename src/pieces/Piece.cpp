#include "Piece.h"
#include <fstream>
#include <sstream>
#include <string>

const std::shared_ptr<Shape> Piece::CUBE_SHAPE = std::shared_ptr<Shape>(
                                     new Shape{std::vector<Point3>{ Point3{-Piece::CUBE_SIZE, -Piece::CUBE_SIZE, -Piece::CUBE_SIZE},
                                     Point3{Piece::CUBE_SIZE, -Piece::CUBE_SIZE, -Piece::CUBE_SIZE},
                                     Point3{Piece::CUBE_SIZE, Piece::CUBE_SIZE, -Piece::CUBE_SIZE},
                                     Point3{-Piece::CUBE_SIZE, Piece::CUBE_SIZE, -Piece::CUBE_SIZE},
                                     Point3{Piece::CUBE_SIZE, -Piece::CUBE_SIZE, Piece::CUBE_SIZE},
                                     Point3{Piece::CUBE_SIZE, Piece::CUBE_SIZE, Piece::CUBE_SIZE},
                                     Point3{-Piece::CUBE_SIZE, -Piece::CUBE_SIZE, Piece::CUBE_SIZE},
                                     Point3{-Piece::CUBE_SIZE, Piece::CUBE_SIZE, Piece::CUBE_SIZE}},
                              std::vector<int>{
                                     1, 4, 5, 2, // right face
                                     0, 3, 7, 6, // left face
                                     0, 1, 2, 3, // front face
                                     0, 6, 4, 1}}); // top face

std::vector<int> split(const std::string &txt, char ch)
{
    std::vector<int> strs;

    size_t pos = txt.find( ch );
    size_t initialPos = 0;
    strs.clear();

    // Decompose statement
    while( pos != std::string::npos ) {
        strs.push_back( std::stoi( txt.substr( initialPos, pos - initialPos ) ) );
        initialPos = pos + 1;

        pos = txt.find( ch, initialPos );
    }

    // Add the last one
    strs.push_back( std::stoi( txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) ) );

    return strs;
}


Piece::Piece(Renderer* renderer, const std::string& shape) : Transform{nullptr}
{
    setRenderer(renderer);

    float z = 0.0f;
    float size = Piece::CUBE_SIZE * 2;

    std::ifstream infile("resources/pieces/" + shape + ".piece");
    std::string line;
    while(std::getline(infile, line)) {
        float x = 0.0f;
        for (int height : split(line, ' ')) {
            int y = 0;

            /* Negative values means that the cube should be placed at that
            height without filling the space below it */
            if (height < 0) {
                height = -height;
                y = height - 1;
            }
            for (; y < height; y++) {
                Transform* cube = new Transform{Piece::CUBE_SHAPE};
                cube->setPosition(Point3{x, -y * size, z}); // -y: positive value in the file means the cube is higher
                cube->setColor(SDL_Color{255, 0, 0});
                addChild(cube);

                m_cubes.push_back(std::unique_ptr<Transform>(cube));
            }

            x += size;
        }

        z -= size; // comes towards the player
    }
}


Piece::~Piece()
{
    //dtor
}
