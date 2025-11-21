#pragma once
#include "Src/Actor/Stage/IStage.h"
#include <vector>

class DimensionTrigger;
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
    void TriggerInstace();
    void WallActorInstace();
private:
    CollisionObject* pCollisionObject_ = nullptr;
    DimensionTrigger* pDimensionTrigger_ = nullptr;
    WallActor* pWallActor_ = nullptr;

};
