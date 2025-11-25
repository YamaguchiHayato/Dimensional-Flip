#pragma once
#include "Src/Actor/Stage/IStage.h"
#include <vector>

class Boss;
class DimensionTrigger;
class Player;
class WallActor;
class Stage3 : public IStage
{
public:
    Stage3() {};
    virtual ~Stage3() {};

    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;
    inline Vector3 GetStageStartPos() const override
    {
        return Vector3::Zero;
    }


private:
    inline void BossInstance();
    void TriggerInstace();
    void WallActorInstace();

private:
    Player* pPlayer_ = nullptr;
    Boss* pBoss_ = nullptr;
    CollisionObject* pCollisionObject_ = nullptr;
    DimensionTrigger* pDimensionTrigger_ = nullptr;
    WallActor* pWallActor_ = nullptr;

private:
    FontRender debugFont_x;
    FontRender debugFont_y;
    FontRender debugFont_z;
};
