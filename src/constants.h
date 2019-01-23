#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED
#include <string>

/** app constants */
const std::string ORG_NAME = "com.alephzero";
const std::string APP_NAME = "harentz";

/** Gameplay constants */
static constexpr int NUMBER_OF_LANES = 5;
static constexpr int SCORE_PER_PIECE = 10;

static constexpr int INITIAL_WIDTH = 410;
static constexpr int INITIAL_HEIGHT = 700;

/** 3D rendering constants */
static constexpr float SCREEN_Z = 0.0f;
static constexpr float PROJECTION_POINT_Z = -500.0f;

/** Bonus malus constants */
static constexpr float BONUS_MALUS_TEXT_DURATION = 1500.0f;
static constexpr float INVINCIVILITY_DURATION = 1000.0f;

/** Sound constants */
static constexpr int NUM_CHANNELS = 16;

#endif // CONSTANTS_H_INCLUDED
