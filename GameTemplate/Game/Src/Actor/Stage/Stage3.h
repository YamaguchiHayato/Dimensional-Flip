#pragma once
#include "Src/Actor/Stage/IStage.h"
#include <vector>

class Boss;
class DimensionTrigger;
class Player;
class WallActor;
class Star;
class Stage3 : public IStage
{
public:
    Stage3() = default;
    virtual ~Stage3();

    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;
    inline Vector3 GetStageStartPos() const override
    {
        return Vector3::Zero;
    }

    void RequestRotStage(float angle, const Vector3& pivot);

private:
    inline void BossInstance();
    void TriggerInstace();
    void WallActorInstace();
    void StarInstance();

private:
    Player* pPlayer_ = nullptr;
    Boss* pBoss_ = nullptr;
    CollisionObject* pCollisionObject_ = nullptr;

    // ポインタ。
    // p … ポインタ。
    DimensionTrigger* pDimensionTrigger_ = nullptr;
    Star* pStar_ = nullptr;
    WallActor* pWallActor_ = nullptr;

    // 配列化。
    // l … リスト。
    std::vector<WallActor*> lWallActor_;
    std::vector<DimensionTrigger*> lDimensionTrigger_;
    std::vector<Star*> lStar_;

private:
    ///////////////////////
    // デバッグ用。////////
    FontRender debugFont_x;
    FontRender debugFont_y;
    FontRender debugFont_z;
    ///////////////////////

    bool isRotating_ = false;          // 回転中フラグ。
    float targeteAngle_ = 0.0f;        // 目標角度。
    float currentAngle_ = 0.0f;        // 現在の角度。
    Vector3 rotPivot_ = Vector3::Zero; // 回転中心。
    Vector3 initialStagePos_ = Vector3::Zero;
    Quaternion initialStageRot_ = Quaternion::Identity;

};
