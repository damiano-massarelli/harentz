#include "BonusMalusPiece.h"
#include "GameScene.h"
#include "DisplayManager.h"
#include "Mat4.h"
#include "LinearTransition.h"

BonusMalusPiece::BonusMalusPiece(Renderer* renderer) : Piece{renderer, "C"}
{
    /* Starts listening for enterframe events */
    m_enterFrameCrumb = DisplayManager::getInstance()->getEventManager().addListenerFor(EventManager::ENTER_FRAME_EVENT, this, true);

    /* Reduce the size of the piece */
    Mat4 scaleMatrix = scale(0.5, 0.5, 0.5);
    getChildren()[0]->setTransformationMatrix(scaleMatrix);

    setFillColor(SDL_Color{0, 255, 0, 128});
}

void BonusMalusPiece::handleCollision(int collidedCubeIndex)
{
    /* translate the cube upwards */
    Point3 collisionPosition = getWorldPosition();
    LinearTransition<float>::create(getPosition().y,
                                    getPosition().y - 5.0f * (Piece::getCubeSide()),
                                    [this, &collisionPosition](float y) {
                                            Point3 newPosition = collisionPosition;
                                            newPosition.y = y;
                                            this->setPosition(newPosition);
                                    },
                                    250.0f,
                                    nullptr,
                                    "game");

    /* Change the alpha of the cube */
    LinearTransition<int>::create(255,
                                   0,
                                   [this](int a) {
                                         SDL_Color fillColor = this->getFillColor();
                                         fillColor.a = a;
                                         this->setFillColor(fillColor);

                                         SDL_Color outlineColor = this->getOutlineColor();
                                         outlineColor.a = a;
                                         this->setOutlineColor(outlineColor);
                                    },
                                    250.0f,
                                    nullptr,
                                    "game");
}

void BonusMalusPiece::onEvent(SDL_Event e)
{
    float delta = (*(static_cast<Uint32*>(e.user.data1)));
    Mat4 tranformMatrix = rotation(0.0f, m_yRotation, 0.0f) * scale(0.5f, 0.5f, 0.5f);
    m_yRotation += 6.28f * delta/1000.0f; // One complete rotation per second
    if (getChildren().size() > 0)
        getChildren()[0]->setTransformationMatrix(tranformMatrix);
}


BonusMalusPiece::~BonusMalusPiece()
{

}
