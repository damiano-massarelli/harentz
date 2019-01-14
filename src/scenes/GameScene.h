#ifndef GAMESCENE_H
#define GAMESCENE_H
#include "Scene.h"
#include "Renderer.h"
#include "Piece.h"
#include <memory>
#include "Ground.h"
#include "Point3.h"
#include "Mat4.h"
#include "Player.h"
#include "PieceManager.h"
#include "StarField.h"
#include "Text.h"
#include "constants.h"
#include <SDL_gpu.h>
#include <vector>
#include <iostream>

class GameScene : public Scene
{
    private:
        /* Pieces and ground use two different renderers so that pieces are always drawn above the ground */
        std::unique_ptr<Renderer> m_3dRenderer;
        std::unique_ptr<Renderer> m_effectRenderer;
        std::vector<std::unique_ptr<Piece>> m_pieces;
        std::unique_ptr<Ground> m_ground;
        std::unique_ptr<Player> m_player;

        std::unique_ptr<PieceManager> m_pieceManager;

        std::unique_ptr<StarField> m_starFieldEffect;

        std::unique_ptr<Text> m_scoreText; ///< the text containing the current score

        Point3 m_spawnPoint; ///< the point where new pieces are spawned, only y and z are relevant.
        Mat4 m_rotationMatrix; ///< the matrix used by all the rotated objects

        int m_score = 0; ///< the score of the player

        bool m_paused = false; ///< true when the application is paused (e.g. put in bg in android)

    public:
        GameScene();

        virtual void onShow(GPU_Target* screen) override;

        virtual void onEnterFrame(SDL_Event& e) override;

        virtual void onRenderingComplete() override;

        virtual void onPause(const EventStatus& status) override;

        virtual void onResume(const EventStatus& status) override;

        /** \brief A recursive function that displays a countdown text
          * when the countdown is over the game is resumed ( \sa m_paused)
          * The recursive call is delayed using a LinearTransition */
        void startResumeCountdown(int countdown, std::shared_ptr<Text> countdownText) const;

        virtual void onRemove() override;

        PieceManager* getPieceManager();

        StarField* getStarFieldEffect();

        Renderer* getEffectRenderer();

        Player* getPlayer();

        void incrementScore(int inc = SCORE_PER_PIECE);

        int getScore() const;

        virtual ~GameScene();

};

#endif // GAMESCENE_H
