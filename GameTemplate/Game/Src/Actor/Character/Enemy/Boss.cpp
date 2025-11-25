#include "stdafx.h"
#include "Src/Actor/Character/Enemy/Boss.h"
#include "Src/Actor/Character/Player.h"



bool Boss::Start()
{
    SetAnimation();
    render_.Init("Assets/modelData/enemy/boss.tkm", animationClip_, BossAnimation::bossAnim_Num, enModelUpAxisZ);
    
    render_.SetScale(Vector3::One);
    render_.SetPosition(pos_);

    charaCon_.Init(50.0f, 150.0f, pos_);
    initPos_ = pos_;


    pPlayer_ = FindGO<Player>("player");
    return true;
}

void Boss::Update()
{
    PlayAnimatipn();
    Rotaition();
    render_.SetRotation(rot_);
    render_.SetScale(Vector3::One);
    render_.SetPosition(pos_);
    render_.Update();
}

void Boss::Render(RenderContext& rc)
{
    render_.Draw(rc);
}

void Boss::Attack(Player* target)
{

}

void Boss::Rotaition()
{
    Vector3 dir = moveSpeed_;
    if (fabsf(dir.x) >= 0.001f || fabsf(dir.z) >= 0.001f)
    {
        rot_.SetRotationYFromDirectionXZ(dir);
        render_.SetRotation(rot_);
    }
}

void Boss::PlayAnimatipn()
{
    switch (state_)
    {

     case BossAnimation::bossAnim_Idle:         
         render_.PlayAnimation(BossAnimation::bossAnim_Idle, 0.1f);
         break;

    case BossAnimation::bossAnim_Run:
        render_.PlayAnimation(BossAnimation::bossAnim_Run, 0.1f);
        break;

    case BossAnimation::bossAnim_Attack:
        render_.PlayAnimation(BossAnimation::bossAnim_Attack, 0.1f);
        break;

    case BossAnimation::bossAnim_Hit:
        render_.PlayAnimation(BossAnimation::bossAnim_Hit, 0.1f);
        break;
    }
}

const std::string Boss::GetAnimation(BossAnimation animName , const std::string animationName, bool flag)
{
    const std::string AnimationFilePath = "Assets/animData/boss/" + animationName + ".tka";
    animationClip_[animName].Load(AnimationFilePath.c_str());
    animationClip_[animName].SetLoopFlag(flag);

    return AnimationFilePath;
}

void Boss::SetAnimation()
{
    //// 各種アニメーションの設定
    GetAnimation(BossAnimation::bossAnim_Idle, "idle", true);
    // 攻撃アニメーション。
    GetAnimation(BossAnimation::bossAnim_Attack, "attack", true);
    // 追跡アニメーション。
    GetAnimation(BossAnimation::bossAnim_Run, "run", true);
    // ダメージアニメーション。
    GetAnimation(BossAnimation::bossAnim_Hit, "damage", false);
}

void Boss::Move()
{
    // 移動中。
    if (state_ == 1)
    {
        moveSpeed_.x = 3.0f;
    }
}
