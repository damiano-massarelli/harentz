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
        static const std::shared_ptr<Shape> CUBE_SHAPE;

        std::vector<std::unique_ptr<Transform>> m_cubes;


    public:
        static constexpr int CUBE_SIZE = 20.0f;

        Piece(Renderer* renderer, const std::string& shape);

        virtual ~Piece();

};

#endif // PIECE_H
