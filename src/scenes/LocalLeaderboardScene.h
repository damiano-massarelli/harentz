#ifndef LOCALLEADERBOARDSCENE_H
#define LOCALLEADERBOARDSCENE_H
#include "Scene.h"
#include "Text.h"
#include <SDL_gpu.h>
#include <memory>
#include <vector>

class LocalLeaderboardScene : public Scene
{
    private:
        std::vector<std::unique_ptr<Text>> m_scoreTexts;
        std::unique_ptr<Text> m_gameOverText;
        int m_lastScore;

        std::vector<int> loadScore() const;

        void saveScore(const std::vector<int>& scores);

    public:
        LocalLeaderboardScene(int score);

        virtual void onShow(GPU_Target* screen) override;

        virtual void onRemove() override;

        virtual ~LocalLeaderboardScene();
};

#endif // LOCALLEADERBOARDSCENE_H
