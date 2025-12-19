#pragma once
#include "Src/camera/ICameraStrategy.h"

class Player;
class SideCameraStrategy : public ICameraStrategy
{
public:
    // 暗黙的な型変換を禁止する。
	explicit SideCameraStrategy(Player* pPlayer);
    virtual ~SideCameraStrategy() {};


public:
	bool Start()override { return true; };
	void Update() override;


private:
	Quaternion currentRotation_ = Quaternion::Identity;
};
