#ifndef INITIALINSTRUCTIONS_H
#define INITIALINSTRUCTIONS_H
#include "AbstractRenderable.h"
#include "Button.h"
#include "Text.h"
#include "memoryUtils.h"
#include <SDL_gpu.h>
#include <memory>
#include <functional>

/** \brief displays the initial instructions as well as some separators during gameplay */
class Instructions : public AbstractRenderable
{
    private:
        GPU_Target* m_screen;

        imageUniquePtr m_imageSeparator;
        imageUniquePtr m_imageArrow;
        std::unique_ptr<Button> m_okButton;
        std::unique_ptr<Text> m_explanationText;

        bool m_firstPlay{true};
        std::function<void()> m_instructionOverCallback;

        void fadeOutInstructions();

    public:
        Instructions(GPU_Target* screen, std::function<void()> onInstructionOver);

        bool checkFirstPlay();

        virtual void render() override;

        virtual ~Instructions();
};

#endif // INITIALINSTRUCTIONS_H
