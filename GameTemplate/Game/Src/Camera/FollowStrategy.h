#pragma once
#include "Src/camera/ICameraStrategy.h"

namespace app{
    namespace enemy{
        class Boss;
    }
}

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
    // ステージ用カメラワーク。
    void StageCamera();

    // ボス戦用のカメラワーク。
    void BossCamera();
 
    // 足場を登る際の視点を作る。
    void MakeClimbingPerspective();


private:
    app::enemy::Boss* pBoss_ = nullptr; 


private:
    //直前の地面に居たときのY座標を保存。
    float lastGroundY_ = 0.0f;

    float orbitAngleY_ = 0.0f;
    float orbitAngleX_ = 0.0f;

    bool isScreenRock_ = false;
    bool isBossTumbler_ = false; // ボスが疲労ステートに入ったかどうか。

    Vector3 rangeMin_ = Vector3::Zero;
    Vector3 rangeMax_ = Vector3::Zero;

    // 足場を登る際のオフセット。
    Vector3 backOffset_ = Vector3::Zero;
    Vector3 frontOffset_ = Vector3::Zero;
    Vector3 finalOffset_ = Vector3::Zero;

    Vector3 idealPos_ = Vector3::Zero; // 理想的な位置
    Vector3 lookAtPoint_ = Vector3::Zero;
    Vector3 targetPos_ = Vector3::Zero;
    Quaternion currentPlayerRot_ = Quaternion::Identity;
};
