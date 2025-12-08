#include "stdafx.h"
#include "Src/Actor/Character/Enemy/Boss.h"
#include "Src/Actor/Character/Player.h"
#include "BossState.h"

namespace
{
    const float INTERVAL = 4.0f;

}

 const std::string Boss::GetAnimation(BossAnimation animName, const std::string animationName, bool flag)
        {
            const std::string AnimationFilePath = "Assets/animData/boss/" + animationName + ".tka";
            animationClip_[animName].Load(AnimationFilePath.c_str());
            animationClip_[animName].SetLoopFlag(flag);

            return AnimationFilePath;
        }


 bool Boss::Start()
 {
     SetAnimation();

     render_.Init("Assets/modelData/enemy/boss.tkm", animationClip_, BossAnimation::bossAnim_Num, enModelUpAxisZ);

     render_.SetPosition(pos_);

     charaCon_.Init(50.0f, 150.0f, pos_);
     initPos_ = pos_;

     rot_.AddRotationDegY(270.0f);
     render_.SetRotation(rot_);

     pPlayer_ = FindGO<Player>("player");

     return true;
 }


 void Boss::Update()
 {
     // ステートをコントロールする。
     ControlState();

     //現在のステートに合わせてアニメーションを再生支持する。
     PlayAnimation();
     Rotaition();


     render_.SetRotation(rot_);
     render_.SetScale(Vector3(2.0f, 2.0f, 2.0f));
     render_.SetPosition(pos_);
     render_.Update();
 }


 void Boss::Render(RenderContext& rc)
        {
            render_.Draw(rc);
        }


 void Boss::Attack(Player* target) {}


 void Boss::Rotaition()
 {
     Vector3 dir = moveSpeed_;

     if (fabsf(dir.x) >= 0.001f || fabsf(dir.z) >= 0.001f)
     {
         rot_.SetRotationYFromDirectionXZ(dir);
         render_.SetRotation(rot_);
     }
 }


 void Boss::PlayAnimation()
 {
     switch (state_)
     {

     // 待機モーション。
     case BossAnimation::bossAnim_Idle:
         render_.PlayAnimation(BossAnimation::bossAnim_Idle, 0.1f);
         break;

     // 走るモーション。
     case BossAnimation::bossAnim_Run:
         render_.PlayAnimation(BossAnimation::bossAnim_Run, 0.1f);
         break;

     // 攻撃モーション(1パターン目。)。
     case BossAnimation::bossAnim_AttackCast:
         render_.PlayAnimation(BossAnimation::bossAnim_AttackCast, 0.1f);
         break;

     // 攻撃モーション(2パターン目。)。    
     case BossAnimation::bossAnim_AttackRoar:
         render_.PlayAnimation(BossAnimation::bossAnim_AttackRoar, 0.1f);
         break;

     // ダメージヒットモーション。
     case BossAnimation::bossAnim_Hit:
         render_.PlayAnimation(BossAnimation::bossAnim_Hit, 0.1f);
         break;
     }
 }


 void Boss::ControlState()
 {
     switch (state_)
     {
     case BossAnimation::bossAnim_Idle:
     {
         stateTimer_ += g_gameTime->GetFrameDeltaTime();

         // stateTimer_ の設定値の分だけカウントが進むと、攻撃が開始される。
         if (stateTimer_ >= INTERVAL)
         {
             uint8_t randomNum = rand() % (uint8_t) AttackType::Num;
             currentAttackType_ = static_cast<AttackType>(randomNum);

             // 攻撃の種類に合わせて次に再生するアニメーションを決める。
             if (currentAttackType_ == AttackType::Roar)
                 // 咆哮の場合。
                 state_ = BossAnimation::bossAnim_AttackRoar;

             // 隕石、追従攻撃の場合。
             else
                 state_ = bossAnim_AttackCast;

             // タイマーをリセットする。
             stateTimer_ = 0.0f;
         }
         break;
     }

     // 攻撃状態(Cast or Roarの場合。)
     case BossAnimation::bossAnim_AttackCast:
     case BossAnimation::bossAnim_AttackRoar:
     {
         // アニメーション再生が終わったかチェックする。
         if (render_.IsPlayingAnimation() == false)
         {
             // 攻撃ステートが終わったら待機状態に戻る。
             state_ = BossAnimation::bossAnim_Idle;
             stateTimer_ = 0.0f;
         }
         break;
     }

     default:
         break;
     }
 }


 void Boss::SetAnimation()
 {
     //// 各種アニメーションの設定
     GetAnimation(BossAnimation::bossAnim_Idle, "idle", true);
     // 追跡アニメーション。
     GetAnimation(BossAnimation::bossAnim_Run, "run", true);
     // 攻撃アニメーション(隕石、槍用)。
     GetAnimation(BossAnimation::bossAnim_AttackCast, "attack", false);
     // 咆哮アニメーション。
     GetAnimation(BossAnimation::bossAnim_AttackRoar, "roar", false);
     // ダメージアニメーション。
     GetAnimation(BossAnimation::bossAnim_Hit, "damage", false);
 }
