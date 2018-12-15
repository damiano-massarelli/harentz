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

        int m_horizontalCubes{0};

        /** \brief reads the color of this piece from file.
          * \param shape the file containing the color is "resources/pieces/" + shape + ".color"
          * \return the read color */
        SDL_Color readPieceColor(const std::string& shape);

    public:
        /** \brief returns the size of a side of the cube */
        static float getCubeSide();

        Piece(Renderer* renderer, const std::string& shape);

        /** \brief returns the x coordinate for lane */
        float xForLane(int lane) const;

        /** \brief gets the number of cubes this piece is composed of */
        int getNumOfHorizontalCubes() const;

        /** \brief removes one of the cubes of this piece */
        void removeCube(int index);

        virtual ~Piece();

};

#endif // PIECE_H
