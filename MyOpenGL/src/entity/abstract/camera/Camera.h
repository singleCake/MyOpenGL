#pragma once
#include "../AbstractEntity.h"
#include "../ViewPoint.h"
#include "../../../constant/CameraConstant.h"

#include <iostream>

class Camera : public AbstractEntity {
	friend class ViewPoint;
public:
	enum ViewpointMode { Euler, Child, All };

	Camera();
	Camera(ViewPoint* vp) : Camera() { viewpoint = vp; }
	Camera(const CVector& pos, const CVector& vel, const CVector& axis);

	void update(float dt) override;
	void handleKeyInput(int keyCode);
	void handleMouseOffset(double dx, double dy);
	
	void setFront(const CVector& front) { this->front = front; euler = front.toEuler(); }
	void setUp(const CVector& up = { 0, 1, 0 }) { this->up = up; }
	const CVector& getFront() const { return front; }
	const CVector& getUp() const { return up; }
	const CVector& getRight() const { return front.crossMul(up).normalize(); }
	const CMatrix& getView() const { return view; }
	virtual const char* getID() const = 0;

	// 绑定视点
	void bindViewPoint(ViewPoint* vp) { viewpoint = vp; }

	// 输出调试信息
	static void switchDebug() { DEBUG_INFO = !DEBUG_INFO; }
	// 切换视角
	virtual void switchMode() {}
	// 切换视点模式
	static void changeViewpointMode();

protected:
	// 用于实时绑定视点
	virtual void updateCamera();

	ViewPoint* viewpoint;
	const CVector& getVPCenter() const { return viewpoint->center; }
	const CVector& getVPUp() const { return viewpoint->up; }
	const CVector& getVPFront() const { return viewpoint->front; }
	const CVector& getVPToward() const { return viewpoint->getToward(); }
	const float getVPOffset() const { return viewpoint->offset; }

private:
	static bool DEBUG_INFO;
	static int VIEWPOINT_MODE;

	float cvel, sensitivity = DEFAULT_CAMERA_SENSITIVITY;
	CVector front, up;
	CEuler euler;
	CMatrix view;

	void moveForward(float d) { setPos(getPos() + getFront() * d); }
	void moveRight(float d) {setPos(getPos() + getRight() * d); }

	void updateView();
};