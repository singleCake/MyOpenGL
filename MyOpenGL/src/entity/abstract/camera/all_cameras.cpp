#include "all_cameras.h"

const char* DefaultCamera::ID = "Default";
const char* PlayerCamera::ID = "Player";
const char* SpaceCraftCamera::ID = "SpaceCraft";

void DefaultCamera::updateCamera() {
	setPos(getVPCenter() + getVPUp() * getVPOffset());
	setUp(getVPUp());
}

void PlayerCamera::updateCamera() {
	switch (mode)
	{
	case PlayerCamera::FirstPerson:
		setPos(getVPCenter() + getVPUp() * (PLAYER_SIZE * 1.01f) + getVPToward() * (PLAYER_SIZE * 0.51f));
		Camera::updateCamera();
		break;
	case PlayerCamera::SecondPerson:
		setPos(getVPCenter() + getVPUp() * (PLAYER_SIZE * 1.01f) + getVPFront() * (PLAYER_SIZE * 4.5f));
		setUp(getVPUp());
		setFront(-getVPFront());
		break;
	case PlayerCamera::ThridPerson:
		setPos(getVPCenter() + getVPUp() * (PLAYER_SIZE * 3.01f) + -getVPToward() * (PLAYER_SIZE * 1.51f));
		Camera::updateCamera();
		break;
	}
}

void SpaceCraftCamera::updateCamera() {
	switch (mode)
	{
	case SpaceCraftCamera::Default:
		setPos(getVPCenter()
			+ getVPUp() * (DEFAULT_SPACE_CRAFT_SIZE * LENGTH_SCALE)
			- getVPToward() * (DEFAULT_SPACE_CRAFT_SIZE * LENGTH_SCALE * 4));
		Camera::updateCamera();
		break;
	case SpaceCraftCamera::LookInside:
		setPos(getVPCenter() + getVPUp() * (DEFAULT_SPACE_CRAFT_SIZE * LENGTH_SCALE * 2));
		setFront(-getVPUp());
		setUp(getVPToward());
		break;
	}
}