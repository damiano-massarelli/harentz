#ifndef PIECE_H
#define PIECE_H
#include "AbstractRenderable.h"
#include "Renderer.h"
#include "Shape.h"
#include "Transform.h"
#include <vector>

class Piece : public AbstractRenderable
{
    private:
        static const Shape CUBE_SHAPE;

        Renderer* m_renderer = nullptr;
        std::vector<Transform> m_cubes;


    public:
        static constexpr int CUBE_SIZE = 20.0f;

        Piece(Renderer* renderer, const std::string& shape);

        virtual void render() override {
            if (m_renderer != nullptr) {
                for (const auto cubeTransform : m_cubes)
                    m_renderer->render(cubeTransform);
            }
        }

        virtual ~Piece();


};

#endif // PIECE_H
