#pragma once
#include "Src/camera/ICameraStrategy.h"

class Player;
class FollowStrategy : public ICameraStrategy
{
public:
    // 暗黙的な型変換を禁止する。
	explicit FollowStrategy(Player* pPlayer);
	virtual ~FollowStrategy() {};


public:
	bool Start() override;
	void Update() override;


public:
    // カメラの制限範囲を設定。
    inline void SetCameraLimit(const Vector3& rangeMin, const Vector3 rangeMax)
    {
        rangeMin_ = rangeMin;
        rangeMax_ = rangeMax;
        isScreenRock_ = true;
    }

    // スクリーンロック。
    void ApplyScreenRock(Vector3& cameraPos);

private:
    //直前の地面に居たときのY座標を保存。
    float lastGroundY_ = 0.0f;

    bool isScreenRock_ = false;
    Vector3 rangeMin_ = Vector3::Zero;
    Vector3 rangeMax_ = Vector3::Zero;
};
