#ifndef CREDITSSCENE_H
#define CREDITSSCENE_H
#include "Scene.h"
#include "Text.h"
#include "Button.h"
#include <SDL_gpu.h>
#include <memory>

class CreditsScene : public Scene
{
    private:
        std::unique_ptr<Text> m_title;
        std::unique_ptr<Text> m_credits;

        std::unique_ptr<Button> m_backButton;

    public:
        CreditsScene();

        virtual void onShow(GPU_Target* scree) override;

        virtual ~CreditsScene();
};

#endif // CREDITSSCENE_H
