#include "LocalLeaderboardScene.h"
#include "DisplayManager.h"
#include "colorUtils.h"
#include "LinearTransition.h"
#include "ioUtils.h"
#include "stringUtils.h"
#include "Button.h"
#include "GameScene.h"
#include "FntParser.h"
#include "CreditsScene.h"
#include <string>
#include <sstream>

LocalLeaderboardScene::LocalLeaderboardScene(int score) : m_lastScore{score}
{
    //ctor
}

std::vector<int> LocalLeaderboardScene::loadScore() const
{
    std::stringstream savedScore = readFile("scores.ldb", true);
    if (savedScore.bad()) // not saved scores yet
        return std::vector<int>{};
    return split<int>(savedScore.str(), ',', [](std::string& str) { return std::stoi(str); });
}

void LocalLeaderboardScene::saveScore(const std::vector<int>& scores)
{
    std::stringstream toSave;
    for (auto it = scores.begin(); it != scores.end(); ++it) {
        if (it != scores.begin()) toSave << ",";
        toSave << *it;
    }
    writeFile("scores.ldb", toSave.str(), true);
}

void LocalLeaderboardScene::onShow(GPU_Target* screen)
{
    Scene::onShow(screen);
    // credit button
    m_creditsButton = std::make_unique<Button>(screen, "resources/font/invasion2000", "credits", 20.0f, 20.0f);
    m_creditsButton->setX((DisplayManager::screenWidth() - m_creditsButton->getWidth())/2);
    m_creditsButton->setY(DisplayManager::screenHeight() - m_creditsButton->getHeight() - 10);
    m_creditsButton->setOnClick([](Button* btn){DisplayManager::getInstance()->setCurrentScene(new CreditsScene{});});
    //add(m_creditsButton.get()); this button is disabled until android renders the screen decently

    // game over text
    m_gameOverText = std::make_unique<Text>(screen, "resources/font/invasion2000_50");
    m_gameOverText->setText("game over");
    m_gameOverText->setY(10);
    m_gameOverText->setX((DisplayManager::screenWidth() - m_gameOverText->getWidth())/2);
    add(m_gameOverText.get());

    // play again button
    m_playAgainButton = std::make_unique<Button>(screen, "resources/font/invasion2000_50", "play again", 20, 20,
                                                SDL_Color{0, 0, 0, 0}, SDL_Color{0, 0, 0, 0}, SDL_Color{213, 48, 241, 255});
    m_playAgainButton->setX((DisplayManager::screenWidth() - m_playAgainButton->getWidth())/2);
    m_playAgainButton->setY(DisplayManager::screenHeight() - m_playAgainButton->getHeight()*2);
    m_playAgainButton->setOnClick([](Button* btn){
                                  DisplayManager::getInstance()->setCurrentScene(new GameScene{});});
    add(m_playAgainButton.get());

    /* the number of scores that fit in the space between game over and play again */
    int numOfScores = (m_playAgainButton->getY() - (m_gameOverText->getHeight() + 10 + m_gameOverText->getY())) / (FntParser::load("resources/font/invasion2000").getLineHeight() + 5);
    numOfScores &= ~1; // rounds down to nearest even number

    auto scores = loadScore();
    if (m_lastScore > 0)
        scores.push_back(m_lastScore);
    std::sort(scores.begin(), scores.end(), [](int a, int b) {return b < a;});
    scores.resize(numOfScores);    // shows only the first tot scores
    saveScore(scores);

    float textPosY = m_gameOverText->getY() + m_gameOverText->getHeight() + 10;
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
