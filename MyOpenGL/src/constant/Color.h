#pragma once

#include "../math/CVector.h"

/*=======����=======*/
constexpr CVector WHITE = { 1.0f, 1.0f, 1.0f };
constexpr CVector BLACK = { 0.0f, 0.0f, 0.0f };
constexpr CVector RED = { 0.6f, 0.0f, 0.0f };
constexpr CVector GREEN = { 0.0f, 0.6f, 0.0f };
constexpr CVector BLUE = { 0.0f, 0.0f, 0.6f };
constexpr CVector YELLOW = { 0.6f, 0.6f, 0.0f };
constexpr CVector GREY = { 0.3f, 0.3f, 0.3f };

/*=======����ͼԪ=======*/
constexpr CVector BORDER_COLOR = { 0.1f, 0.1f, 0.1f };

/*=======ʵ����ϵ=======*/
constexpr CVector HOVER_COLOR = { 0.6f, 0.6f, 0.6f };
constexpr CVector SELECT_COLOR = { 0.6f, 0.6f, 0 };
constexpr CVector DEFAULT_ORBIT_COLOR = { 0.15f, 0.15f, 0.15f };

/*=======�ɴ�=======*/
constexpr CVector BODY_COLOR = { 0.4f, 0.4f, 0.4f };
constexpr CVector HEAD_COLOR = { 0.8f, 0.0f, 0.0f };
constexpr CVector TAIL_COLOR = { 0.8f, 0.7f, 0.0f };
constexpr CVector WING_COLOR = { 0.8f, 0.8f, 0.0f };