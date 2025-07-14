#pragma once

#include "../geometry/Geometry.h"
#include <array>

class LightManager {
public:
	static void init();

	static void enableAllLights();
	static void disableAllLights();

	static void update(float dt);
	static void initCabinLight(CVector* cabin);
	static void initSpotLight(CVector* spotPos, CVector* spotDir);

	static void toggleCabinLight();
	static void toggleSpotLight();

private:
	static GLfloat sunPos[];
	static CVector* cabinPos;
	static CVector* spotPos;
	static CVector* spotDir;

	static std::array<int, 3> allLights;
	static std::array<bool, 3> lightEnabled;
	static std::array<bool, 3> lightAvailable;

	static void initSunLight();
};