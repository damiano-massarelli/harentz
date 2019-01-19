#include "LocalLeaderboardScene.h"
#include "DisplayManager.h"
#include "colorUtils.h"
#include "LinearTransition.h"
#include <string>
#include <sstream>

LocalLeaderboardScene::LocalLeaderboardScene(int score) : m_lastScore{score}
{
    //ctor
}

std::vector<int> LocalLeaderboardScene::loadScore() const
{
    return std::vector<int>{};
}

void LocalLeaderboardScene::saveScore(const std::vector<int>& scores)
{

}

void LocalLeaderboardScene::onShow(GPU_Target* screen)
{
    Scene::onShow(screen);

    int numOfScores = 10; // TODO compute this value

    auto scores = loadScore();
    scores.push_back(m_lastScore);
    std::sort(scores.begin(), scores.end(), [](int a, int b) {return b < a;});
    scores.resize(numOfScores);    // shows only the first tot scores



    float textPosY = 20;
    float textPosX = 10.0f;
    for (std::size_t i = 0; i < scores.size(); ++i) {
        int score = scores[i];

        /* Creates a string like:
         * pos............score */
        auto text = std::make_unique<Text>(screen, "resources/font/invasion2000");
        text->setText(std::to_string(i + 1) + " " + std::to_string(score));
        int spacePerDot = text->getFntParser().getDataForChar(static_cast<int>('.'))->xadvance;
        int numOfDots = (DisplayManager::screenWidth() - textPosX - text->getWidth()) / (spacePerDot);

        std::stringstream decoratedScore;
        decoratedScore << i + 1;
        for (int j = 0; j < numOfDots; j++)
            decoratedScore << ".";
        decoratedScore << score;

        text->setText(decoratedScore.str());
        text->setX(textPosX);
        text->setY(textPosY);

        /* Changes the alpha of each text. Each transition starts 75m after the previous one */
        float colorBlendFactor = i/(static_cast<float>(numOfScores));
        SDL_Color textColor;
        if (score == m_lastScore)
            textColor = SDL_Color{255,251,150,0};
        else
            textColor = SDL_Color{1,205,254,0} * colorBlendFactor + SDL_Color{185,103,255,0} * (1.0f - colorBlendFactor);
        text->setColor(textColor);
        LinearTransition<int>::create(0, 0, nullptr, 75.0f * i, [text = text.get(), textColor]{ // first transition to delay
                                      LinearTransition<Uint8>::create(0, 255, [text, textColor](Uint8 a) { // second transition actually changes alpha
                                                                        SDL_Color c = textColor;
                                                                        c.a = a;
                                                                        text->setColor(c);
                                                                      }, 700.0f, nullptr, "game");
                                      }, "game");
        textPosY += text->getHeight() + 5;

        add(text.get());
        m_scoreTexts.push_back(std::move(text));
    }
}

void LocalLeaderboardScene::onRemove()
{
    // deletes all the transitions generated. This is necessary as these transitions operate
    // on objects that will be destroyed
    DisplayManager::getInstance()->getTransitionManager().cancelByTag("game");
}

LocalLeaderboardScene::~LocalLeaderboardScene()
{
    //dtor
}
