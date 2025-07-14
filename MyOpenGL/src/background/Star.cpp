#include "Star.h"
#include <GLFW/glfw3.h>

Star::Star()
	:t(0), color(CVector(1, 1, 1)), position(CVector(0, 0, 0)), size(1), vel(0.1), clockwise(false) {
	createStar();
}

Star::Star(float phi, const CVector& position, float size, float vel, bool clockwise)
	: t(phi), position(position), size(size), vel(vel), clockwise(clockwise) {
	createStar();
}

void Star::createStar() {
	float luminance = (sin(t) + 1) / 2;
	color = CVector(luminance, luminance, luminance);

	r = sqrt(position.x * position.x + position.z * position.z);
}

void Star::set(float phi, const CVector& position, float size, float vel, bool clockwise) {
	this->t = phi;
	this->position = position;
	this->size = size;
	this->vel = vel;
	this->clockwise = clockwise;
	float luminance = (sin(phi) + 1) / 2;
	color = CVector(luminance, luminance, luminance);

	r = sqrt(position.x * position.x + position.z * position.z);
}

void Star::update(float dt) {
	float omega = 2;
	t += dt * omega;
	float luminance = (sin(t) + 1) / 2;
	color.set(luminance, luminance, luminance);

	if (r > 0.00000) {
		int flag = clockwise ? 1 : -1;
		float x = position.x, z = position.z;
		r = sqrt(x * x + z * z);
		position.x += flag * vel * z / r * dt;
		position.z += -flag * vel * x / r * dt;
	}
}

Star::~Star() {}

void Star::draw() {
	glBegin(GL_POINTS);
	glPointSize((GLfloat)size);
	glColor3fv(color);
	glVertex3fv(position);
	glEnd();
}