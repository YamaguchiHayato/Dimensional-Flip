/*
 * @file ICameraBase.h
 * @brief カメラ基底クラス
 * @dote 2025/10/27 … クラス作成日。
 */
#pragma once
namespace nsK2EngineLow {
	class Camera;
}

class ICameraStrategy
{
public:
	ICameraStrategy() = default;
	virtual ~ICameraStrategy() = default;
	virtual bool Start() = 0;
	
    virtual void Update
	(nsK2EngineLow::Camera* pCamera, const float deltaTime) = 0;
};
