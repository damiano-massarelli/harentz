#ifndef PIECE_H
#define PIECE_H
#include "Renderer.h"
#include "Shape.h"
#include "Transform.h"
#include <vector>
#include <memory>
#include <SDL2/SDL.h>

class Piece : public Transform
{
    private:
        static std::shared_ptr<Shape> CUBE_SHAPE;

        static std::shared_ptr<Shape> getCubeShape();

        std::vector<std::unique_ptr<Transform>> m_cubes;

        /** \brief reads the color of this piece from file.
          * \param shape the file containing the color is "resources/pieces/" + shape + ".color"
          * \return the read color */
        SDL_Color readPieceColor(const std::string& shape);


    public:
        static float getCubeSide();

        Piece(Renderer* renderer, const std::string& shape);

        virtual ~Piece();

};

#endif // PIECE_H
