#include "InitialInstructions.h"
#include "DisplayManager.h"
#include "LinearTransition.h"

InitialInstructions::InitialInstructions(GPU_Target* screen) :
                                             m_screen{screen},
                                             m_okButton{screen, "resources/font/invasion2000", "got it.", 10, 10},
                                             m_explanationText{screen, "resources/font/invasion2000"}
{
    m_explanationText.setText("tap left and right to move.\n tap in the middle to jump");
    m_explanationText.setX((DisplayManager::screenWidth() - m_explanationText.getWidth())/2);
    m_explanationText.setY(DisplayManager::screenHeight()/2 - m_explanationText.getHeight());

    m_okButton.setX((DisplayManager::screenWidth() - m_okButton.getWidth())/2);
    m_okButton.setY(m_explanationText.getY() + m_explanationText.getHeight() + 10);

    m_imageLeftRight = imageUniquePtr{GPU_LoadImage("resources/images/instructions.png")};
    GPU_SetAnchor(m_imageLeftRight.get(), 1.0f, 1.0f);

    m_imageJump = imageUniquePtr{GPU_LoadImage("resources/images/jump_instructions.png")};
    GPU_SetAnchor(m_imageJump.get(), 0.5f, 1.0f);

    LinearTransition<Uint8>::create(0, 255, [this](Uint8 a) {
                                        GPU_SetRGBA(this->m_imageLeftRight.get(), 255, 255, 255, a);
                                        GPU_SetRGBA(this->m_imageJump.get(), 255, 255, 255, a);
                                    },
                                   750.0f, nullptr, "game");
}

void InitialInstructions::setOnOk(std::function<void(Button*)> onOk)
{
    m_okButton.setOnClick(onOk);
}

void InitialInstructions::render()
{
    GPU_RectangleFilled(m_screen, 0, 0, DisplayManager::screenWidth(), DisplayManager::screenHeight(), SDL_Color{0, 0, 0, 180});
    GPU_Blit(m_imageLeftRight.get(), nullptr, m_screen, DisplayManager::screenWidth()/3.0f, DisplayManager::screenHeight());
    GPU_BlitScale(m_imageLeftRight.get(), nullptr, m_screen, 2*DisplayManager::screenWidth()/3.0f, DisplayManager::screenHeight(), -1.0f, 1.0f);
    GPU_Blit(m_imageJump.get(), nullptr, m_screen, DisplayManager::screenWidth()/2.0f, DisplayManager::screenHeight());
    m_explanationText.render();
    m_okButton.render();
}

InitialInstructions::~InitialInstructions()
{
}
