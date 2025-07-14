#pragma once

/*=======星系=======*/
constexpr float DEFAULT_AUTO_ROTATION = 0.3f;
constexpr float LINE_BALL_SCALE = 1.2f;
constexpr float SATURN_RING_INNER_SCALE = 1.5f;
constexpr float SATURN_RING_OUTER_SCALE = 2.0f;

constexpr float DEFAULT_PLANET_RADIUS = 0.5f;
constexpr float SUN_RADIUS = DEFAULT_PLANET_RADIUS * 7.5f;
constexpr float MOON_RADIUS = DEFAULT_PLANET_RADIUS * 0.5f;

constexpr float ORBIT_RADIUS = 2.5f;
constexpr float MOON_ORBIT_SCALE = 1.5f; // 根据地球半径缩放

constexpr float DEFAULT_PLANET_VEL = 2.5f;
constexpr float MOON_VEL_SCALE = 2.0f;

/*=======飞船=======*/
constexpr float TAIL_OFFSET_SCALE = 1.2f;
constexpr float TAIL_LENGTH_SCALE = 0.8f;
constexpr float TAIL_WIDTH_SCALE = 0.5f;
constexpr float LENGTH_SCALE = 1.4f;
constexpr float LIGHT_SCALE = 0.5f;
constexpr float HEAD_RADIUS_SCALE = 0.75f;
constexpr float HEAD_HEIGHT_SCALE = 1.5f;
constexpr float WING_LENGTH_SCALE = 0.7f;
constexpr float WING_WIDTH_SCALE = 0.8f * LENGTH_SCALE;
constexpr float WING_SIZE = 0.02f;

constexpr float SENSITIVITY = 0.08f;

constexpr float DEFAULT_SPACE_CRAFT_SPEED = 0.3f;
constexpr float DEFAULT_SPACE_CRAFT_SIZE = 0.3f;

/*=======玩家=======*/
constexpr float PLAYER_SIZE = DEFAULT_SPACE_CRAFT_SIZE / 12;
constexpr float PLAYER_WIDTH = PLAYER_SIZE;
constexpr float PLAYER_HEIGHT = PLAYER_SIZE * 3;
constexpr float PLAYER_BODY_LENGTH = PLAYER_SIZE * 0.3f;
constexpr float PLAYER_BODY_WIDTH = PLAYER_SIZE * 0.8f;
constexpr float PLAYER_BODY_HEIGHT = PLAYER_SIZE;
constexpr float PLAYER_ARM_RADIUS = PLAYER_SIZE * 0.1f;
constexpr float PLAYER_ARM_OFFSET = PLAYER_BODY_WIDTH * 0.51f + PLAYER_ARM_RADIUS;
constexpr float PLAYER_LEG_RADIUS = PLAYER_BODY_WIDTH * 0.12f;
constexpr float PLAYER_LEG_OFFSET = PLAYER_BODY_WIDTH * 0.25f;

constexpr float PLAYER_DEFAULT_SPEED = 0.03f;

constexpr float PLAYER_ARM_OMEGA = 0.8f;
constexpr float PLAYER_ARM_MAX_ANGLE = 0.5f;
