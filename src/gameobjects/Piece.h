#ifndef PIECE_H
#define PIECE_H
#include "Renderer.h"
#include "Shape.h"
#include "Transform.h"
#include <vector>
#include <memory>
#include <SDL.h>
#include <utility>
#include <set>

class Piece : public Transform
{
    private:
        static std::shared_ptr<Shape> CUBE_SHAPE;

        static std::shared_ptr<Shape> getCubeShape();

        std::vector<std::unique_ptr<Transform>> m_cubes;

        std::set<std::tuple<int, int, int>> m_cubeAtPosition; ///< an element is present if a cube is present at the given offset

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

        /** \brief handles the collision with the player */
        virtual void handleCollision(int collidedCubeIndex);

        /** \brief checks whether a cube is present in (x, y, z)
          * x, y, z are offset and not 3d coordinates. They represent
          * the position of a cube as it is stored in the file */
        bool cubeAt(int x, int y, int z) const;

        virtual ~Piece();

};

#endif // PIECE_H
