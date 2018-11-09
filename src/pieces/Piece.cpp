#include "Piece.h"
#include <fstream>
#include <sstream>
#include <string>

const Shape Piece::CUBE_SHAPE(std::vector<Point3>{ Point3{-Piece::CUBE_SIZE, -Piece::CUBE_SIZE, -Piece::CUBE_SIZE},
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
                                     0, 6, 4, 1}); // top face

std::vector<int> split(const std::string &txt, char ch)
{
    std::vector<int> strs;

    size_t pos = txt.find( ch );
    size_t initialPos = 0;
    strs.clear();

    // Decompose statement
    while( pos != std::string::npos ) {
        std::cout << txt.substr( initialPos, pos - initialPos ) << "-\n";
        strs.push_back( std::stoi( txt.substr( initialPos, pos - initialPos ) ) );
        initialPos = pos + 1;

        pos = txt.find( ch, initialPos );
    }

    // Add the last one
    std::cout << txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) << "-\n";
    strs.push_back( std::stoi( txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) ) );

    return strs;
}


Piece::Piece(Renderer* renderer, const std::string& shape) : m_renderer{renderer}
{
    float z = 0.0f;
    float x = 0.0f;
    float y = 0.0f;

    std::ifstream infile(shape);
    std::string line;
    while(std::getline(infile, line)) {
        x = 0.0f;
        for (int height : split(line, ' ')) {
            //m_cubes.push_back(Transform{Piece::CUBE_SHAPE, Point3{x, y+100.0f, z}});

            x += Piece::CUBE_SIZE;
        }

        z += Piece::CUBE_SIZE;
    }
}


Piece::~Piece()
{
    //dtor
}
