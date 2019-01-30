#ifndef INITIALINSTRUCTIONS_H
#define INITIALINSTRUCTIONS_H
#include "AbstractRenderable.h"
#include "Button.h"
#include "Text.h"
#include <SDL_gpu.h>
#include <memory>
#include <functional>

class InitialInstructions : public AbstractRenderable
{
    private:
        struct ImageDeleter {
            void operator()(GPU_Image* image) {
                GPU_FreeImage(image);
            }
        };
        using imageUniquePtr = std::unique_ptr<GPU_Image, ImageDeleter>;
        GPU_Target* m_screen;

        imageUniquePtr m_imageLeftRight;
        imageUniquePtr m_imageJump;
        Button m_okButton;
        Text m_explanationText;

    public:
        InitialInstructions(GPU_Target* screen);

        void setOnOk(std::function<void(Button*)> onOk);

        virtual void render() override;

        virtual ~InitialInstructions();
};

#endif // INITIALINSTRUCTIONS_H
