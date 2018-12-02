#ifndef PIECE_H
#define PIECE_H
#include "Renderer.h"
#include "Shape.h"
#include "Transform.h"
#include <vector>
#include <memory>

class Piece : public Transform
{
    private:
        static std::shared_ptr<Shape> CUBE_SHAPE;

        static std::shared_ptr<Shape> getCubeShape();

        std::vector<std::unique_ptr<Transform>> m_cubes;


    public:
        static float getCubeSide();

        Piece(Renderer* renderer, const std::string& shape);

        virtual ~Piece();

};

#endif // PIECE_H
