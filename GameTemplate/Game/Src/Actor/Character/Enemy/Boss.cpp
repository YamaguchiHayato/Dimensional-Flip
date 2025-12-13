#include "stdafx.h"
#include "Src/Actor/Character/Enemy/Boss.h"
#include "Src/Actor/Character/Player.h"
#include "BossState.h"

// ボス戦時のギミッククラス。
#include "Src/Actor/Stage/Gimmick/Meteo.h"
#include "Src/Actor/Stage/Gimmick/Spear.h"
#include "Src/Actor/Stage/Gimmick/FloatingPlatform.h"



namespace
{
    // 攻撃インターバル時間。
    const auto INTERVAL = 4.0f;

    // 次の攻撃までのインターバルを管理する変数。
    const auto NEXT_INTERVAL = 4.0f;

    // アニメーション再生後から何秒後に生成するか。
    // モーションに合わせて調整する。
    const auto ATTACK_TARGET_TIME = 0.8f;

    // 隕石を生成する個数。
    const uint8_t METEO_SPAWN_NUM = 6;

    // 何回攻撃したら疲れるか 
    const uint8_t ATTACK_LIMIT_TO_TIRED = 5;

    // 生成範囲。
    const auto RANGE = 450.0f;

    // 疲労状態（足場が出ている）の時間
    const auto TIRED_DURATION = 20.0f;

    // アニメーションデータを保持。
    AnimationClip ANIMCLIP[BossAnimation::bossAnim_Num];
    // 読み込み中かどうかのフラグ。
    bool isLoad = false;

    // コリジョンの半径。
    const float WEEKE_POINT_RADIUS = 200.0f;
}


// 足場の生成座標を管理する構造体。
namespace ScaffoldingPosList
{

    const std::vector<std::vector<Vector3>> SCAFFOLDING_PATTERNS =
    {
        // --- パターンA: スタンダードな階段 ---
        // プレイヤーの目の前から、ボスの懐まで真っ直ぐ登る
        {
            Vector3(0.0f, 50.0f, -250.0f), 
            Vector3(0.0f, 100.0f, -150.0f),
            Vector3(0.0f, 150.0f, -50.0f),
            Vector3(0.0f, 200.0f,  50.0f),
            Vector3(0.0f, 250.0f, 150.0f), 
        },

        // --- パターンB: ジグザグ配置 (小) ---
        // 横幅を以前の半分(150)にして、小さいステージでもはみ出さないように調整
        {
            Vector3(0.0f, 50.0f, -250.0f),   // 中央
            Vector3(150.0f, 100.0f, -150.0f), // 右へ
            Vector3(-150.0f, 150.0f, -50.0f), // 左へ
            Vector3(150.0f, 200.0f, 50.0f),   // 右へ
            Vector3(0.0f, 250.0f, 150.0f),    // 中央（ボス前）
        },

        // --- パターンC: 急上昇 ---
        // ボスの近くで一気に高さを稼ぐパターン
        {
            Vector3(0.0f, 50.0f, -200.0f),
            Vector3(0.0f, 100.0f, -120.0f),
            Vector3(0.0f, 150.0f, -40.0f),
            Vector3(0.0f, 200.0f,  40.0f),
            Vector3(0.0f, 250.0f, 120.0f),
        }
    };
};



bool Boss::Start()
 {
     SetAnimation();

     render_.Init("Assets/modelData/enemy/boss.tkm", ANIMCLIP, BossAnimation::bossAnim_Num, enModelUpAxisZ);
     render_.SetPosition(pos_);
     initPos_ = pos_;
     rot_.AddRotationDegY(270.0f);
     render_.SetRotation(rot_);
     pPlayer_ = FindGO<Player>("player");

     pWeeekPoint_ = NewGO<CollisionObject>(0);

     pWeeekPoint_->CreateSphere(pos_ + Vector3(0.0f, 300.0f, 50.0f), Quaternion::Identity, WEEKE_POINT_RADIUS);
     pWeeekPoint_->SetIsEnableAutoDelete(false);
     pWeeekPoint_->SetIsEnable(false);


     pFloatingPlatform_.clear();
     for (int i = 0; i < 5; ++i)
     {
         auto platform = NewGO<app::gimmick::FloatingPlatform>(0);
         platform->SetName("Scaffolding");
         pFloatingPlatform_.push_back(platform);
     }

     return true;
 }


 void Boss::Update()
 {
     // ステートをコントロールする。
     ControlState();

     //現在のステートに合わせてアニメーションを再生指示する。
     PlayAnimation();
     Rotaition();

     if (pWeeekPoint_)
         pWeeekPoint_->SetPosition(pos_ + Vector3(0.0f, 200.0f, 0.0f));

     render_.SetRotation(rot_);
     render_.SetScale(Vector3::One);
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

     // 転倒モーション。
     case BossAnimation::bossAnim_Tumble:
         render_.PlayAnimation(BossAnimation::bossAnim_Tumble, 0.1f);
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
             // 攻撃終了後。
             attackCount_++;

             // 規定回数分攻撃を行うと疲労状態へ移行する。
             if (attackCount_ >= ATTACK_LIMIT_TO_TIRED)
             {
                 // 疲労状態へ移行して足場を出す
                 state_ = BossAnimation::bossAnim_Tumble;
                 attackCount_ = 0;
                 stateTimer_ = 0.0f;

                 SpawnPlatforms();
             }
             else
             {
                 // まだ元気なら通常通りIdleへ
                 state_ = BossAnimation::bossAnim_Idle;
                 stateTimer_ = 0.0f;

                 // 次の攻撃までのインターバル時間をランダムで設定。
                 nextInterval_ = 2.0f + (static_cast<float>(rand() % 16) / 10.0f);
             }
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


     ////////////////////////////////////
     // 疲労状態。///////////////////////
     ////////////////////////////////////
     case BossAnimation::bossAnim_Tumble:
     {
         pWeeekPoint_->SetIsEnable(true);
         canBeAttacked_ = true;

         // 経過時間で回復させる。
         stateTimer_ += g_gameTime->GetFrameDeltaTime();

         // ボス頭上付近にいるかチェックする。
         if (canBeAttacked_ && pPlayer_ != nullptr)
         {
             // ボス頭上付近の座標。
             // @TODO : ボスの頭(攻撃可能な判定) の位置を設定する。 
             Vector3 headPos = pos_ + Vector3(0.0f, 200.0f, 0.0f);

             // プレイヤーの位置。
             Vector3 playerPos = pPlayer_->GetPlayerPos();

             Vector3 diff = playerPos - headPos;
             float distXZ = sqrtf(diff.x * diff.x + diff.z * diff.z);
             bool isAbove = (diff.y > 0.0f);

             if (distXZ < 150.0f && isAbove)
             {
                 // 弱点にヒット。
                 canBeAttacked_ = false;
                 pWeeekPoint_->SetIsEnable(false);

                 // 足場を消す。
                 for (auto p : pFloatingPlatform_)
                 {
                     if (p != nullptr)
                         p ->Deactivate();
                 }

                 hp--;

                 // ダメージモーションへ遷移する。
                 state_ = BossAnimation::bossAnim_Hit;
                 stateTimer_ = 0.0f;
                 break;
             }


         }

         // 疲労時間が終了したら
         if (stateTimer_ >= TIRED_DURATION)
         {

             for (auto p : pFloatingPlatform_)
             {
                 if (p != nullptr)
                     p->Deactivate();
             }

             pWeeekPoint_->SetIsEnable(false);
             canBeAttacked_ = false;

             // 通常状態へ戻る
             state_ = BossAnimation::bossAnim_Idle;
             stateTimer_ = 0.0f;
             nextInterval_ = 3.0f;
         }
         break;
     }


     ////////////////////////////////////
     // ダメージ状態。///////////////////
     ////////////////////////////////////
     case BossAnimation::bossAnim_Hit:
     {
         // アニメーション再生が終わったかチェックする。
         if (!render_.IsPlayingAnimation())
         {
             if (hp <= 0)
             {
                 // 倒れた後の処理。
             }

             else
             {
                 // HPがまだあるなら通常状態へ遷移する。
                 state_ = BossAnimation::bossAnim_Idle;
                 nextInterval_ = 3.0f;
             }

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
     // ロードしていなければここでロードする。
     if (!isLoad)
     {
         // デフォルト状態。
         ANIMCLIP[BossAnimation::bossAnim_Idle].Load("Assets/animData/boss/idle.tka");
         ANIMCLIP[BossAnimation::bossAnim_Idle].SetLoopFlag(true);


         // 走る状態。
         ANIMCLIP[BossAnimation::bossAnim_Run].Load("Assets/animData/boss/run.tka");
         ANIMCLIP[BossAnimation::bossAnim_Run].SetLoopFlag(true);


         // 攻撃状態。
         ANIMCLIP[BossAnimation::bossAnim_AttackCast].Load("Assets/animData/boss/attack.tka");
         ANIMCLIP[BossAnimation::bossAnim_AttackCast].SetLoopFlag(false);


         // 咆哮攻撃状態。
         ANIMCLIP[BossAnimation::bossAnim_AttackRoar].Load("Assets/animData/boss/roar.tka");
         ANIMCLIP[BossAnimation::bossAnim_AttackRoar].SetLoopFlag(false);


         // ダメージヒット状態。
         ANIMCLIP[BossAnimation::bossAnim_Hit].Load("Assets/animData/boss/damage.tka");
         ANIMCLIP[BossAnimation::bossAnim_Hit].SetLoopFlag(true);


         // 転倒状態。
         ANIMCLIP[BossAnimation::bossAnim_Tumble].Load("Assets/animData/boss/week.tka");
         ANIMCLIP[BossAnimation::bossAnim_Tumble].SetLoopFlag(false);

         // ロード完了フラグを立てる
         isLoad = true;
     }

 }


void Boss::SpawnPlatforms()
 {
    // 足場の生成パターンをランダムに設定する。
    uint8_t patternIndex = rand() % ScaffoldingPosList::SCAFFOLDING_PATTERNS.size();

    // 選ばれた座標リストへの参照を取得。
    const std::vector<Vector3>& currenPattern = ScaffoldingPosList::SCAFFOLDING_PATTERNS[patternIndex];

     // 配列のサイズ分だけループ
     size_t count = min(pFloatingPlatform_.size(), currenPattern.size());
     for (size_t i = 0; i < count; i++)
     {
         if (pFloatingPlatform_[i])
             pFloatingPlatform_[i]->Activate(currenPattern[i]);
     }
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

