#include "stdafx.h"

// キャラクター。
#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Player/Player.h"

// コア。
#include "Src/Core/BattlePhaseManager.h"
#include "Src/Core/BossUIManager.h"
#include  "Src/Core/InputManager.h"

// データクラス。
#include "Src/Actor/Character/Enemy/Boss/BossType.h"


// ステートクラス。
#include "Src/Actor/Character/Enemy/Boss/BossIdleState.h"
#include "Src/Actor/Character/Enemy/Boss/BossAttackState.h"
#include "Src/Actor/Character/Enemy/Boss/BossDamageState.h"
#include "Src/Actor/Character/Enemy/Boss/BossTumbleState.h"    

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
    const auto RANGE = 20.0f;

    // 疲労状態（足場が出ている）の時間
    const auto TIRED_DURATION = 20.0f;

    // コリジョンの半径。
    const float WEEKE_POINT_RADIUS = 5.0f;

    const auto WEAK_POINT_HEIGHT = 22.0f;

    // 攻撃可能なコリジョン。
    const Vector3 CAN_ATTACK_COLLISION_POSITION = Vector3(0.0f, WEAK_POINT_HEIGHT, 0.0f);

    const float STAGE_LIMIT_X = 35.0f;
}


// 足場の生成座標を管理する構造体。
namespace ScaffoldingPosList
{

    const std::vector<std::vector<Vector3>> SCAFFOLDING_PATTERNS =
    {
        // --- パターンA: スタンダードな階段 ---
        // プレイヤーの目の前から、ボスの懐まで真っ直ぐ登る
        {
            Vector3(0.0f, 40.0f, -250.0f),
            Vector3(0.0f, 60.0f, -190.0f),
            Vector3(0.0f, 80.0f, -130.0f),
            Vector3(0.0f, 100.0f, -70.0f),
            Vector3(0.0f, 120.0f, -20.0f),
        },

        // --- パターンB: ジグザグ配置 (小) ---
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


namespace app
{
    namespace enemy
    {
        Boss::~Boss()
        {
            // メモリ解放。
            for (uint8_t i = 0; i < app::enemyStatus::BossState::state_Num; ++i)
            {
                if (pStateList_[i])
                {
                    delete pStateList_[i];
                    pStateList_[i] = nullptr;
                }
            }

            // InputManagerの次元反転フラグを元に戻す。
            pInputManager_ = app::core::InputManager::GetInstance();
            if (pInputManager_)
                pInputManager_->SetDimensionFlipFlag(true);
        }


        bool Boss::Start()
        {
            // ステートの登録。
            // 待機ステート。
            RegisterState<app::enemyState::BossIdleState>(app::enemyStatus::state_Idle);
            // 攻撃ステート。
            RegisterState<app::enemyState::BossAttackState>(app::enemyStatus::state_Attack);
            // 疲労ステート。
            RegisterState<app::enemyState::BossTumbleState>(app::enemyStatus::state_Tumble);
            // ダメージステート。
            RegisterState<app::enemyState::BossDamageState>(app::enemyStatus::state_Hit);

            // アニメーションをセットする。
            SetAnimation();

            // モデルをセットする。
            render_.Init("Assets/modelData/enemy/boss.tkm", animClips_, app::enemyStatus::BossAnimation::bossAnim_Num, enModelUpAxisZ);
            render_.SetPosition(pos_);

            // ボーンIDを取得。
            weakPointBoneID_ = render_.FindBoneID(L"Head");

            rot_.AddRotationDegY(-90.0f);
            render_.SetRotation(rot_);
            pPlayer_ = FindGO<Player>("player");

            pWeeekPoint_ = NewGO<CollisionObject>(0);

            pWeeekPoint_->CreateSphere(pos_ + Vector3(0.0f, 300.0f, 50.0f), Quaternion::Identity, WEEKE_POINT_RADIUS);
            pWeeekPoint_->SetIsEnableAutoDelete(false);
            pWeeekPoint_->SetIsEnable(false);


            // 初期ステートの登録。
            pCurrentState_ = pStateList_[app::enemyStatus::state_Idle];
            if (pCurrentState_)
                pCurrentState_->Enter();

            // フェーズマネージャの初期化。
            app::core::BattlePhaseManager::GetInstance()->Init();

            // InputManagerの次元反転フラグを無効化。
            app::core::InputManager::GetInstance()->SetDimensionFlipFlag(false);


            return true;
        }


        void Boss::Update()
        {
            // ステートが存在することを確認。
            _ASSERT(pCurrentState_ != nullptr);

            // ステートの更新処理。
            uint8_t request;
            if (pCurrentState_->RequestID(request))
            {
                pCurrentState_->Exit();
                pCurrentState_ = pStateList_[request];
                pCurrentState_->Enter();
            }
            pCurrentState_->Update();

            // PhaseManagerの更新。
            app::core::BattlePhaseManager::GetInstance()->Update();

            // カメラのClamp制限。
            AddClamp();

            // 回転。
            Rotaition();

            // 攻撃可能をコリジョンの位置。
            if (pWeeekPoint_)
                pWeeekPoint_->SetPosition(GetWeakPoint());

            // モデル本体の更新。
            render_.SetRotation(rot_);
            render_.SetScale(Vector3(0.15f, 0.15f, 0.15f));
            render_.SetPosition(pos_);

            // UIにHPの情報を通知。
            app::nsUI::BossUIManager::GetInstance().OnUpdateHP(static_cast<float>(hp), 3.0f);

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


        void Boss::AddClamp()
        {
            // 2Dフェーズ時の移動制限。
            auto* pPhase = app::core::BattlePhaseManager::GetInstance()->GetCurrentPhase();
            if (*pPhase == app::enemyStatus::BossPhase::phase_One)
            {
                // 座標を固定。
                pos_.z = 0.0f;
                moveSpeed_.z = 0.0f;

                // カメラ範囲を超えないように制限。
                if (pos_.x < -STAGE_LIMIT_X)
                    pos_.x = -STAGE_LIMIT_X;

                if (pos_.x > STAGE_LIMIT_X)
                    pos_.x = STAGE_LIMIT_X;
            }
        }


        void Boss::SetAnimation()
        {
             // デフォルト状態。
             animClips_[app::enemyStatus::BossAnimation::bossAnim_Idle].Load("Assets/animData/boss/idle.tka");
             animClips_[app::enemyStatus::BossAnimation::bossAnim_Idle].SetLoopFlag(true);

             // 走る状態。
             animClips_[app::enemyStatus::BossAnimation::bossAnim_Run].Load("Assets/animData/boss/run.tka");
             animClips_[app::enemyStatus::BossAnimation::bossAnim_Run].SetLoopFlag(true);

             // 攻撃状態。
             animClips_[app::enemyStatus::BossAnimation::bossAnim_AttackCast].Load("Assets/animData/boss/attack.tka");
             animClips_[app::enemyStatus::BossAnimation::bossAnim_AttackCast].SetLoopFlag(false);

             // 咆哮攻撃状態。
             animClips_[app::enemyStatus::BossAnimation::bossAnim_AttackRoar].Load("Assets/animData/boss/roar.tka");
             animClips_[app::enemyStatus::BossAnimation::bossAnim_AttackRoar].SetLoopFlag(false);

             // ダメージヒット状態。
             animClips_[app::enemyStatus::BossAnimation::bossAnim_Hit].Load("Assets/animData/boss/damage.tka");
             animClips_[app::enemyStatus::BossAnimation::bossAnim_Hit].SetLoopFlag(false);

             // 転倒状態。
             animClips_[app::enemyStatus::BossAnimation::bossAnim_Tumble].Load("Assets/animData/boss/week.tka");
             animClips_[app::enemyStatus::BossAnimation::bossAnim_Tumble].SetLoopFlag(true);

             // 着地
             animClips_[app::enemyStatus::BossAnimation::bossAnim_Land].Load("Assets/animData/boss/lamd.tka");
             animClips_[app::enemyStatus::BossAnimation::bossAnim_Land].SetLoopFlag(false);

             // ジャンプ
             animClips_[app::enemyStatus::BossAnimation::bossAnim_Jump].Load("Assets/animData/boss/jump.tka");
             animClips_[app::enemyStatus::BossAnimation::bossAnim_Jump].SetLoopFlag(false);
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

            // Y座標は地面の高さ 
            pos.y = 0.0f;

            return pos;
        }


        Vector3 Boss::GetRandomAttackPos()
        {
            float angle = static_cast<float>(rand() % 360) * (3.14159265f / 180.0f);
            float distance = static_cast<float>(rand() % static_cast<int>(RANGE));
            Vector3 pos = Vector3::Zero;
            pos.x = cosf(angle) * distance;
            pos.z = sinf(angle) * distance;
            pos.y = 0.0f;
            return pos;
        }
    }
}

