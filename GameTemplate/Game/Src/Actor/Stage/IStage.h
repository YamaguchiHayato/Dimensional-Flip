#pragma once
#include "Src/Actor/Actor.h"

// 今がどのステージなのかを識別するするための列挙型。
enum class StageID : uint8_t
{
    sStage1,
    sStage2,
    sStage3,
    sStageEX,
    sInvalid
};


// ステージを管理する基底クラス。
class IStage : public Actor
{
public:
    IStage() {};
    virtual ~IStage() {};
    virtual bool Start() { return true; }
    virtual void Update() {};
    virtual void Render(RenderContext& rc) {};
    virtual const std::string InitStage(const std::string& stagename)
    {
        std::string Stagepath = "Assets/stage/" + stagename + ".tkm";
        return Stagepath;
    };
    // ゲッター。
public:
    virtual Vector3 GetStageStartPos() const = 0 { return Vector3::Zero; };

protected:
    ModelRender stageRender_;

    Vector3 stagePos_ = Vector3::Zero;
    Vector3 initPos_ = Vector3::Zero;
    PhysicsStaticObject stagePhysics_;
};
