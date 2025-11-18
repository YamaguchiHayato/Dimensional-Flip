#include "stdafx.h"
#include "Thwomp.h"
#include "Src/Actor/Character/Enemy/IEnemy.h"
#include "Src/Actor/Character/Player.h"

namespace
{
    const Vector3 SCALE(3.0f, 3.0f, 3.0f);
    const Vector3 BOX_SIZE(250.0f, 250.0f, 50.0f);
}

bool Thwomp::Start()
{
    std::string enemyPath = InitModel("Stage2/Enemy/RotationEnemy");
    render_.Init(enemyPath.c_str());

    pos_ = initPos_;
    rot_ = Quaternion::Identity;

    render_.SetPosition(pos_);
    render_.SetRotation(rot_);
    render_.SetScale(SCALE);
    render_.Update();

    pCollisionObject_ = NewGO<CollisionObject>(0, "thwomp_collision");
    pCollisionObject_->CreateBox(pos_, Quaternion::Identity, BOX_SIZE);
    pCollisionObject_->SetIsEnableAutoDelete(false);

    pPlayer_ = FindGO<Player>("player");
    
    isRespawning_ = false; 
    return true;
}

void Thwomp::Update()
{
const float dt = g_gameTime->GetFrameDeltaTime();

    // 1. 移動処理を呼ぶ
    UpdateMove(dt);

    // 2. 自転処理（ステップ1のまま）
    Quaternion frameRotation;
    frameRotation.SetRotation(
        Vector3::AxisZ,
        Math::DegToRad(spinSpeedDeg_ * dt)
    );
    rot_ = frameRotation * rot_;
    rot_.Normalize();

    // 3. 計算結果をモデルとコリジョンに適用
    // (UpdateMoveで更新された pos_ がセットされる)
    render_.SetPosition(pos_);
    render_.SetRotation(rot_);
    render_.Update(); 

    if (pCollisionObject_ != nullptr)
    {
        pCollisionObject_->SetPosition(pos_);
        pCollisionObject_->SetRotation(rot_);
    }
}

void Thwomp::Render(RenderContext& rc)
{
    // isRespawning_ が false の時だけ描画
    if (!isRespawning_)
    {
        render_.Draw(rc);
    }
}

void Thwomp::InitPos(const Vector3& pos)
{
    pos_ = pos;
    initPos_ = pos;
    rot_ = Quaternion::Identity; 
}

void Thwomp::InitMoveDir(const Vector3& dir)
{
    moveDir_ = dir;
    if (moveDir_.LengthSq() > 0.0f)
    {
        moveDir_.Normalize();
    }
}

void Thwomp::UpdateMove(float deltaTime)
{
    if (isRespawning_) return;

    // moveDir_ (方向) と moveSpeed_ (速さ) に基づいて位置を更新
    if (moveDir_.LengthSq() > 0.001f) // 0より大きいか
    {
        Vector3 dir = moveDir_;
        dir.Normalize(); // 正規化 (重要)
        pos_ += dir * moveSpeed_ * deltaTime;
    }
}
// ★★★ ここにあった余計な } を削除しました ★★★
