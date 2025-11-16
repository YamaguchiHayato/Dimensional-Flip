#pragma once
#include "Src/Actor/Actor.h"
class Player;
class IGimmic : public Actor
{
public:
    IGimmic() {};
    virtual ~IGimmic() {};
    virtual bool Start() override { return true; }
    virtual void Update() override {};
    virtual void Render(RenderContext& rc) override {}

    virtual const std::string InitGimmick(const std::string& gimmickname)
    {
        std::string gimmickpath = "Assets/stage/" + gimmickname + ".tkm";
        return gimmickpath;
    };

// メンバ変数。
protected:
    ModelRender gimmickRender_;
    Vector3 gimmickPos_ = Vector3::Zero;
    Vector3 initGimmickPos_ = Vector3::Zero;

    PhysicsStaticObject gimmickPhysics_;

// コリジョンステータス。
protected:
    const Vector3 collisonHeight_;// コリジョンの高さ
    const Vector3 collisonSize_;// コリジョンの大きさ

// ポインタ。
protected:
    CollisionObject* pGimmickCollision_ = nullptr;

};
