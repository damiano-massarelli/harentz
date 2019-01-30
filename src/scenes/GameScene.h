#ifndef GAMESCENE_H
#define GAMESCENE_H
#include "Scene.h"
#include "Renderer.h"
#include "Piece.h"
#include "Ground.h"
#include "Point3.h"
#include "Mat4.h"
#include "Player.h"
#include "PieceManager.h"
#include "StarField.h"
#include "Text.h"
#include "constants.h"
#include "Instructions.h"
#include <SDL_gpu.h>
#include <vector>
#include <iostream>
#include <memory>

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
        std::unique_ptr<Text> m_livesText; ///< the text containing the current number of lives
        std::unique_ptr<Text> m_messageText; ///< the text containing messages to show to the player (e.g countdown)
        std::unique_ptr<Instructions> m_instructions; ///< instructions shown to the player

        Point3 m_spawnPoint; ///< the point where new pieces are spawned, only y and z are relevant.
        Mat4 m_rotationMatrix; ///< the matrix used by all the rotated objects

        int m_score = 0; ///< the score of the player
        int m_lives = 3; ///< lives of the player

        bool m_paused = true; ///< true when the application is paused (e.g. put in bg in android)
        bool m_gameOver = false;
        std::shared_ptr<LinearTransition<float>> m_resumeCountDown;

    public:
        GameScene();

        virtual void onShow(GPU_Target* screen) override;

        virtual void onEnterFrame(SDL_Event& e) override;

        virtual void onRenderingComplete() override;

        virtual void onPause(const EventStatus& status) override;

        virtual void onResume(const EventStatus& status) override;

        bool isPaused() const;

        /** \brief A recursive function that displays a countdown text
          * when the countdown is over the game is resumed ( \sa m_paused)
          * The recursive call is delayed using a LinearTransition */
        void startResumeCountdown(int countdown);

        virtual void onRemove() override;

        PieceManager* getPieceManager();

        StarField* getStarFieldEffect();

        Renderer* getEffectRenderer();

        Player* getPlayer();

        void incrementScore(int inc = SCORE_PER_PIECE);

        int getScore() const;

        void incrementLives(int lives);

        int getLives() const;

        /** \brief shows a message to the user
         * \param message the message to show
         * \param the time before the message is removed (negative value: do not remove)
         */
        void setMessage(const std::string& message, float duration = -1.0f);

        virtual ~GameScene();

};

#endif // GAMESCENE_H
