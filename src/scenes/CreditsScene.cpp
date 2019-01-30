#include "CreditsScene.h"
#include "DisplayManager.h"
#include "LocalLeaderboardScene.h"

std::string CREDITS = "\033ff71ceffprogramming:\n"
"\03301cdfeff damiano\n\n"
"\033ff71ceffmusic:\n"
"\03301cdfeff Komiku - Battle of Pogs\n"
"(freemusicarchive.org)\n\n"
"\033ff71ceffsound effects:\n"
"\03301cdfeff Juhani Junkala\n"
"(The Essential Retro Video\n"
"Game Sound Effects Collection)\n\n"
"\033ff71ceffmade with:\n"
"\03301cdfeff sdl2 and sdl gpu\n\n";

CreditsScene::CreditsScene()
{
    //ctor
}

void CreditsScene::onShow(GPU_Target* screen)
{
    Scene::onShow(screen);

    m_title = std::make_unique<Text>(screen, "resources/font/invasion2000_50");
    m_title->setText("credits");
    m_title->setY(10);
    m_title->setX((DisplayManager::screenWidth() - m_title->getWidth())/2);
    add(m_title.get(), true);

    m_credits = std::make_unique<Text>(screen, "resources/font/invasion2000");
    m_credits->setText(CREDITS);
    m_credits->setX(10);
    m_credits->setY(m_title->getY() + m_title->getHeight() + 25);
    add(m_credits.get());

    m_backButton = std::make_unique<Button>(screen, "resources/font/invasion2000", "back", 20.0f, 20.0f);
    m_backButton->setX((DisplayManager::screenWidth() - m_backButton->getWidth())/2);
    m_backButton->setY(m_credits->getY() + m_credits->getHeight() + 20.0f);
    add(m_backButton.get());

    m_backButton->setOnClick([](Button* btn) {
                                DisplayManager::getInstance()->setCurrentScene(new LocalLeaderboardScene{-1});
                             });
}

CreditsScene::~CreditsScene()
{
    //dtor
}
