#ifndef MALUSPIECE_H
#define MALUSPIECE_H
#include "Renderer.h"
#include "BonusMalusPiece.h"
#include <map>
#include <SDL.h>

class MalusPiece : public BonusMalusPiece
{
    private:
        enum class Behavior {
            WIREFRAME_ONLY,
            SPEED_UP,
            INVERT_COMMANDS,
            COUNT
        };
        static std::map<Behavior, SDL_Color> behavior2color;

        Behavior m_behavior;

    public:
        MalusPiece(Renderer *renderer);

        virtual void handleCollision(int collidedCubeIndex) override;

        virtual ~MalusPiece();
};

#endif // MALUSPIECE_H
