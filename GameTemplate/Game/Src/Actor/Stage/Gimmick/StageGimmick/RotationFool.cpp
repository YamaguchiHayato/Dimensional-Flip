#include "stdafx.h"

#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Stage/Gimmick/StageGimmick/RotationFool.h"
#include "physics/CollisionAttr.h"
#include "physics/Physics.h"

namespace
{
    const Vector3 SCALE(0.25f, 0.25f, 0.25f);
    const Vector3 COLLISION_SIZE(20.0f, 5.0f, 0.0f);
} // namespace

RotationFool::~RotationFool() {}

bool RotationFool::Start()
{
    std::string foolPath = InitGimmick("Stage2/RotationFool");
    render_.Init(foolPath.c_str());
    render_.SetScale(scale_.x != 1.0f ? scale_ : SCALE);
    render_.SetPosition(gimmickPos_);
    render_.Update();

    pPlayer = FindGO<Player>("player");

    InitPhysics();
    return true;
}

void RotationFool::Update()
{
    Move(g_gameTime->GetFrameDeltaTime());

    render_.SetPosition(gimmickPos_);
    render_.Update();

    // gimmickPhysics_.SetPosition(gimmickPos_);  // ← これでは当たりが動かない
    SyncPhysicsPosition();
}

void RotationFool::Render(RenderContext& rc)
{
    render_.Draw(rc);
}

void RotationFool::InitPhysics()
{
    gimmickPhysics_.CreateBox(gimmickPos_, Quaternion::Identity, COLLISION_SIZE);
    gimmickPhysics_.SetKinematic(true);

    if (auto* body = gimmickPhysics_.GetbtCollisionObject())
        body->setUserIndex(enCollisionAttr_Ground);

    SyncPhysicsPosition();
}

void RotationFool::SyncPhysicsPosition()
{
    btCollisionObject* body = gimmickPhysics_.GetbtCollisionObject();
    if (!body)
        return;

    btTransform trans = body->getWorldTransform();
    trans.setOrigin(btVector3(gimmickPos_.x, gimmickPos_.y, gimmickPos_.z));
    body->setWorldTransform(trans);
    body->activate(true);

    if (auto* world = PhysicsWorld::GetInstance()->GetDynamicWorld())
        world->updateSingleAabb(body);
}

void RotationFool::Move(float deltaTime)
{
    switch (foolState_)
    {
    case FoolState::UP:
        gimmickPos_.y += moveSpeed_ * g_gameTime->GetFrameDeltaTime();
        if (gimmickPos_.y >= GetTopPos().y)
        {
            gimmickPos_.y = GetTopPos().y;
            foolState_ = FoolState::TOP;
            SetStopTime(2.0f);
        }
        break;

    case FoolState::TOP:
        stopTime_ -= deltaTime;
        if (stopTime_ <= 0.0f)
        {
            SetStopTime(2.0f);
            foolState_ = FoolState::DOWN;
        }
        break;

    case FoolState::DOWN:
        gimmickPos_.y -= moveSpeed_ * g_gameTime->GetFrameDeltaTime();
        if (gimmickPos_.y <= GetInitPos().y)
        {
            gimmickPos_.y = GetInitPos().y;
            foolState_ = FoolState::BOTTOM;
            SetStopTime(2.0f);
        }
        break;

    case FoolState::BOTTOM:
        stopTime_ -= deltaTime;
        if (stopTime_ <= 0.0f)
        {
            SetStopTime(2.0f);
            foolState_ = FoolState::UP;
        }
        break;

    default:
        break;
    }
}
