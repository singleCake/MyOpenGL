#pragma once

#include "../entity/planet/Planet.h"
#include "../entity/abstract/camera/DefaultCamera.h"

using CameraUPtr = std::unique_ptr<Camera>;

class CameraManager {
public:
	CameraManager() = default;
	~CameraManager() = default;

	static void init();
	static void update(float dt);

	static void handleKeyInput(int keyCode);
	static void handleMouseOffset(double dx, double dy);

	// 切换相机
	static void switchCamera();
	// 切换相机模式
	static void switchCameraMode() { camera->switchMode(); }
	// 切换视点控制模式
	static void changeViewpointMode() { Camera::changeViewpointMode(); }

	static void bindCameraToPlanet(Planet* planet);

	static void addCamera(CameraUPtr* camera);
	static void removeEmptyCameras();

	static const char* getCameraID() { return camera->getID(); }
	static bool getCameraIDEqualTo(const char* id) { return strcmp(getCameraID(), id) == 0; }
	static const CMatrix& getProjection() { return projection; }
	static const CMatrix& getView() { return camera->getView(); }

private:
	static unsigned int index;
	static CameraUPtr default_camera;
	static std::vector<CameraUPtr*> all_cameras;
	static Camera* camera;

	static CMatrix projection;

	static void myReShape();
};