#pragma once
#include <memory>
#include "Src/camera/ICameraStrategy.h"
#include "Src/Actor/Character/Player.h"
#include "Game.h"

namespace nsK2EngineLow {
	class Camera;
}

enum  class CameraMode  : uint8_t
{
    mode3D,   // 3Dモード。
    mode2_5D, // 2.5Dモード。
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

	void Request2DMode();                        /// 2Dモードへの変更要求
	void Request3Dmode(float targetAngleDegrees);/// 3Dモードでの変更要求

// ゲッター。
public:
	inline CameraMode GetCurrentCameraMode() const { return currentMode_; }
	inline ICameraStrategy* GetCurrentStrategy() { return pCameraStrategy_.get(); };

private:
	std::unique_ptr<ICameraStrategy> pCameraStrategy_;
	CameraMode currentMode_ = CameraMode::mode2_5D; // 初期モード

	Player* pPlayer_ = nullptr; 
};

