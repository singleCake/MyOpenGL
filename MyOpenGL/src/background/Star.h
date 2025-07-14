#pragma once

#include "../math/CVector.h"

class Star {
public:
	Star();
	Star(float phi, const CVector& position, float size, float vel, bool clockwise);
	~Star();
	void set(float phi, const CVector& position, float size, float vel, bool clockwise);
	void update(float dt);
	void draw();

private:
	float t;
	float vel;		//Ĭ��Χ��ԭ����Բ���˶�
	float r;
	bool clockwise;
	CVector color;
	CVector position;
	float size;

	void createStar();
};