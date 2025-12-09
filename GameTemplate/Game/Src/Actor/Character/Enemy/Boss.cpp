#include "stdafx.h"
#include "Src/Actor/Character/Enemy/Boss.h"
#include "Src/Actor/Character/Player.h"
#include "BossState.h"

// 攻撃パターンクラス。
#include "Src/Actor/Stage/Gimmick/Meteo.h"
#include "Src/Actor/Stage/Gimmick/Spear.h"

namespace
{
    // 攻撃インターバル時間。
    const float INTERVAL = 4.0f;

    // 次の攻撃までのインターバルを管理する変数。
    const auto NEXT_INTERVAL = 4.0f;

    // アニメーション再生後から何秒後に生成するか。
    // モーションに合わせて調整する。
    const auto ATTACK_TARGET_TIME = 0.8f;

    // 隕石を生成する個数。
    const uint8_t METEO_SPAWN_NUM = 6;

    // 生成範囲。
    const auto RANGE = 1000.0f;


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

     //現在のステートに合わせてアニメーションを再生指示する。
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


 void Boss::SpawnGimmicks(AttackType type)
 {
     // 共通のループ回数
     for (uint8_t i = 0; i < METEO_SPAWN_NUM; i++)
     {
         // 共通の座標計算
         Vector3 spawnPos = RandomStagePos();

         // タイプごとの生成分岐
         if (type == AttackType::Meteor)
         {
             // --- 隕石の生成 ---

             // 隕石特有のランダムパラメータ
             auto speed = 20.0f + static_cast<float>(rand() % 30);
             auto delay = 0.5f + (static_cast<float>(rand() % 15) / 10.0f);

             auto meteo = NewGO<app::gimmick::Meteo>(0);
             meteo->SetName("meteo");
             meteo->SetTargetPos(spawnPos); // 共通座標をセット
             meteo->SetParams(speed, delay);
         }

         else if (type == AttackType::Spear)
         {
             // --- 槍の生成 ---
             auto spear = NewGO<app::gimmick::Spear>(0);
             spear->SetName("spear");
             spear->SetTargetPos(spawnPos); // 共通座標をセット
         }
     }
 }


 void Boss::ControlState()
 {
     switch (state_)
     {
     //////////////////////////////////
     // 待機状態の場合。////////////////
     /////////////////////////////////
     case BossAnimation::bossAnim_Idle:
     {
         stateTimer_ += g_gameTime->GetFrameDeltaTime();

         // stateTimer_ の設定値の分だけカウントが進むと、攻撃が開始される。
         if (stateTimer_ >= nextInterval_)
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
             isAttackSpawned_ = false;
         }
         break;
     }


     ////////////////////////////////////
     // 攻撃状態(Meteo or Spearの場合。)
     ///////////////////////////////////
     case BossAnimation::bossAnim_AttackCast:
     {
         stateTimer_ += g_gameTime->GetFrameDeltaTime();

         // アニメーションに合わせて攻撃オブジェクトを生成する。
         if (stateTimer_ >= ATTACK_TARGET_TIME && isAttackSpawned_ == false)
         {
             isAttackSpawned_ = true;

             // 攻撃オブジェクトの生成。    
             SpawnGimmicks(currentAttackType_);
         }


         if (render_.IsPlayingAnimation() == false)
         {
             state_ = BossAnimation::bossAnim_Idle;
             stateTimer_ = 0.0f;

             // 次の攻撃までのインターバル時間をランダムで設定。
             nextInterval_ = 2.0f + (static_cast<float>(rand() % 16) / 10.0f);
         }
         break;
     }


     ////////////////////////////////////
     // 攻撃状態(咆哮の場合。)///////////
     //////////////////////////////////
     case BossAnimation::bossAnim_AttackRoar:
     {
         // アニメーション再生が終わったかチェックする。
         if (render_.IsPlayingAnimation() == false)
         {
             state_ = BossAnimation::bossAnim_Idle;
             stateTimer_ = 0.0f;

             // 咆哮は大技なので、次は少し長く休む
             nextInterval_ = 4.0f + (static_cast<float>(rand() % 21) / 10.0f);
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


 Vector3 Boss::RandomStagePos()
 {
     RANGE; // 生成範囲。

     // 1. ランダムな角度 (0 ～ 360度)
     // rand() % 360 で 0~359 の整数を作り、それをラジアンに変換します
     float angle = static_cast<float>(rand() % 360) * (3.14159265f / 180.0f);

     // 2. ランダムな距離 (0 ～ 半径)
     // 中心からどれくらい離れるかをランダムに決めます
     float distance = static_cast<float>(rand() % static_cast<int>(RANGE));

     // 3. 角度と距離から座標を計算 (円の中に配置)
     Vector3 pos = Vector3::Zero;
     pos.x = cosf(angle) * distance;
     pos.z = sinf(angle) * distance;

     // Y座標は地面の高さ (必要に応じて targetPos_.y などに合わせてください)
     pos.y = 0.0f;

     return pos;
 }
