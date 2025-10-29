#pragma once
#include <memory>
#include "Src/camera/ICameraStrategy.h"
#include "Game.h"

namespace nsK2EngineLow {
	class Camera;
}

enum  class CameraMode  : uint8_t
{
	mode3D,
	mode2D
};


class Player;
class SideCameraStrategy;
class CameraManager :  public IGameObject
{
public:
	CameraManager(){};
	virtual ~CameraManager(){};

	bool Start();
	void Update();

	void Request3DMode();                            /// 3Dモードへの変更要求
	void Request2DMode();                            /// 2Dモードへの変更要求
	void Request2DRotation(float targetAngleDegrees);/// 2Dモードでの回転要求

// ゲッター。
public:
	inline CameraMode GetCurrentCameraMode() const { return currentMode_; }
	inline ICameraStrategy* GetCurrentStrategy() { return pCameraStrategy_.get(); };

private:
	std::unique_ptr<ICameraStrategy> pCameraStrategy_;
	CameraMode currentMode_ = CameraMode::mode2D; // 初期モード

	Player* pPlayer_ = nullptr; 
};

