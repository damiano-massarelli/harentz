#include "Instructions.h"
#include "DisplayManager.h"
#include "LinearTransition.h"
#include "ioUtils.h"
#include <sstream>

Instructions::Instructions(GPU_Target* screen, std::function<void()> onInstructionOver) : m_screen{screen},
                            m_instructionOverCallback{onInstructionOver}
{
    m_imageSeparator = imageUniquePtr{GPU_LoadImage("resources/images/separator.png")};
    GPU_SetAnchor(m_imageSeparator.get(), 1.0f, 1.0f);

    m_firstPlay = checkFirstPlay();
    if (m_firstPlay) { // first play: load instruction text and ok button
        m_explanationText = std::make_unique<Text>(screen, "resources/font/invasion2000");
        m_explanationText->setText("tap left and right to move.\n tap in the middle to jump");
        m_explanationText->setX((DisplayManager::screenWidth() - m_explanationText->getWidth())/2);
        m_explanationText->setY(DisplayManager::screenHeight()/2 - m_explanationText->getHeight());

        m_okButton = std::make_unique<Button>(screen, "resources/font/invasion2000", "got it.", 10.0f, 10.0f);

        m_okButton->setX((DisplayManager::screenWidth() - m_okButton->getWidth())/2);
        m_okButton->setY(m_explanationText->getY() + m_explanationText->getHeight() + 10);
        m_okButton->setOnClick([this](Button* btn) {
                                this->m_firstPlay = false;
                                this->fadeOutInstructions();
                               });

        m_imageArrow = imageUniquePtr{GPU_LoadImage("resources/images/arrow.png")};

        LinearTransition<Uint8>::create(0, 255, [this](Uint8 a) {
                                        GPU_SetRGBA(this->m_imageSeparator.get(), 255, 255, 255, a);
                                        GPU_SetRGBA(this->m_imageArrow.get(), 255, 255, 255, a);
                                    },
                                   750.0f, nullptr, "game");
    } else { // this is not the first time, just fade out the separators
        fadeOutInstructions();
    }
}

void Instructions::render()
{
    int width = DisplayManager::screenWidth();
    int height = DisplayManager::screenHeight();

    // renders arrows, text and button. But only if initial instructions should be rendered
    if (m_firstPlay) {
        GPU_RectangleFilled(m_screen, 0, 0, width, height, SDL_Color{0, 0, 0, 180});
        GPU_Blit(m_imageArrow.get(), nullptr, m_screen, width/2.0f, height - m_imageArrow->h/2);
        GPU_BlitRotate(m_imageArrow.get(), nullptr, m_screen, width/3.0f - width/6.0f, height - m_imageArrow->h/2, -90.0f);
        GPU_BlitRotate(m_imageArrow.get(), nullptr, m_screen, 2*width/3.0f + width/6.0f, height - m_imageArrow->h/2, 90.0f);
        m_explanationText->render();
        m_okButton->render();
    }

     // renders the separators
    GPU_Blit(m_imageSeparator.get(), nullptr, m_screen, width/3.0f, height);
    GPU_BlitScale(m_imageSeparator.get(), nullptr, m_screen, 2*width/3.0f, height, -1.0f, 1.0f);
}

void Instructions::fadeOutInstructions()
{
    // fade out the separator and call the instruction over callback
    LinearTransition<Uint8>::create(255, 50, [this](Uint8 a) {
                                        GPU_SetRGBA(m_imageSeparator.get(), 255, 255, 255, a);
                                    }, 750.0f, [this](){
                                        this->m_instructionOverCallback();
                                    }, "game");
}

bool Instructions::checkFirstPlay()
{
    if (readFile("firstTime", true).bad()) {
        writeFile("firstTime", "", true); // saves a file used to check whether the player is playing for the first time
        return true;
    }
    return false;
}

Instructions::~Instructions()
{

}
