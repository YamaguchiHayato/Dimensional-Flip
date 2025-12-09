#include "stdafx.h"
#include "Thwomp.h"
#include "Src/Actor/Character/Player.h"

namespace
{
    const Vector3 SCALE    (3.0f, 3.0f, 3.0f);
    const Vector3 BOX_SIZE (250.0f, 250.0f, 50.0f);
}

Thwomp::~Thwomp()
{
    if (pCollisionObject_ != nullptr)
    {
        DeleteGO(pCollisionObject_);
        pCollisionObject_ = nullptr;
    }
}

bool Thwomp::Start()
{
    // 1. モデル初期化
    std::string enemyPath = InitModel("Stage2/Enemy/RotationEnemy");
    render_.Init(enemyPath.c_str());

    pos_.y += 100.0f;
    initPos_ = pos_;

    render_.SetPosition(pos_);
    render_.SetRotation(rot_);
    render_.SetScale(SCALE);
    render_.Update();

    // 2. 坂道に合わせたコリジョン角度(slopeRot_)の計算
    if (moveDir_.LengthSq() > 0.001f)
    {
        Vector3 dir = moveDir_;
        dir.Normalize();

        float yaw = atan2f(dir.x, dir.z);
        Quaternion qYaw;
        qYaw.SetRotation(Vector3::AxisY, yaw);

        float pitch = -asinf(dir.y); 
        Quaternion qPitch;
        qPitch.SetRotation(Vector3::AxisX, pitch);

        slopeRot_ = qYaw * qPitch;
    }

    else slopeRot_ = Quaternion::Identity;

    // サイズは敵の大きさに合わせて調整 (半径, 高さ, 座標)
    charaCon_.Init(50.0f, 50.0f, pos_);

    // 4. 当たり判定用コリジョン作成 (ゴースト)
    pCollisionObject_ = NewGO<CollisionObject>(0, "thwomp_collision");
    pCollisionObject_->CreateBox(pos_, slopeRot_, BOX_SIZE); 
    pCollisionObject_->SetIsEnableAutoDelete(false);

    // 5. その他
    pPlayer_ = FindGO<Player>("player");
    pos_ = initPos_;
    radius_ = BOX_SIZE.x * 0.5f; 
    isRespawning_ = false;
    
    // 速度初期化
    velocity_ = Vector3::Zero;

    return true;
}

void Thwomp::Update()
{
    const float dt = g_gameTime->GetFrameDeltaTime();

    if (IsActive() == false)
    {
        Vector3 diff = pPlayer_->GetPlayerPos() - triggerPos_;
        if (diff.LengthSq() <= 40000.0f)
            isActive_ = true;
    }

    if (IsActive() == true)
    {
        // 1. 移動・回転計算 (キャラコン処理含む)
        Move(dt);

        // 削除判定。
        if (pos_.x <= triggerPos_.x)
        {
            DeleteGO(this);
            return;
        }
    }

    // 2. モデル更新
    render_.SetPosition(pos_);
    render_.SetRotation(rot_);
    render_.Update();

    // 3. 当たり判定用コリジョンを追従させる
    if (pCollisionObject_ != nullptr)
    {
        Vector3 colliderPos = pos_;
        colliderPos.y += 20.0f; 
        pCollisionObject_->SetPosition(colliderPos);
        // 角度は固定 (回さない)
        pCollisionObject_->SetRotation(slopeRot_); 
    }
}

void Thwomp::Render(RenderContext& rc)
{
    render_.Draw(rc);
}

void Thwomp::Move(float deltaTime)
{
    if (isRespawning_) return;

    // -------------------------------------------------
    // A. 移動速度と重力の計算
    // -------------------------------------------------
    velocity_.x = -moveSpeed_; // X軸マイナス方向へ進む
    velocity_.z = 0.0f;        // Z軸は進まない

    // 接地判定と角度調整
    if (charaCon_.IsOnGround())
    {
        velocity_.y = 0.0f;
        // 地面にいるなら、Startで計算した「斜面の角度」を採用
        if (pCollisionObject_) pCollisionObject_->SetRotation(slopeRot_);
    }
    else
        // 空中なら箱を水平に戻す
        if (pCollisionObject_) pCollisionObject_->SetRotation(Quaternion::Identity);

    // 重力加算
    velocity_.y -= GRAVITY;

    // -------------------------------------------------
    // B. キャラコン移動実行
    // -------------------------------------------------
    pos_ = charaCon_.Execute(velocity_, deltaTime);

    // -------------------------------------------------
    // C. 回転計算 (移動に合わせて Z軸回転)
    // -------------------------------------------------
    float moveDist = velocity_.x * deltaTime;

    if (fabsf(moveDist) > 0.0001f)
    {
        // 回転角度 = 移動距離 / 半径
        float rollAngle = moveDist / radius_;

        Quaternion frameRot;
        frameRot.SetRotation(Vector3::AxisZ, -rollAngle);

        rot_ = frameRot * rot_;
        rot_.Normalize();
    }
}
