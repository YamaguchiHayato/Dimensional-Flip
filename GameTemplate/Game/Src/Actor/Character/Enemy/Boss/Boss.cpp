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
#include "Src/Actor/Character/Enemy/Boss/BossDeadState.h"


namespace
{
    // 生成範囲。
    const auto RANGE = 20.0f;

    // コリジョンの半径。
    const float WEEKE_POINT_RADIUS = 2.5f;

    // コリジョンの高さ。
    const auto WEAK_POINT_HEIGHT = 22.0f;

    // ステージのX方向の制限。
    const float STAGE_LIMIT_X = 35.0f;

    // 最大HP
    const auto MAX_HP = 3.0f;

    const Vector3 SCALE = Vector3(0.15f, 0.15f, 0.15f);

    const float WEAK_POINT_BONE_OFFSET_Y = 4.0f;

    // エフェクトｗセットするための構造体。
    struct EffectResource
    {
        app::enemyStatus::EffectID effectID;
        const char16_t* path;
    };

    // エフェクトリスト。
    const EffectResource resources[] =
    {
        { app::enemyStatus::effect_FireBall, u"Assets/effect/fire.efk"},      // FireBall
        { app::enemyStatus::effect_ShockWave, u"Assets/effect/shockWave.efk"},// ShockWave
        { app::enemyStatus::effect_thunder, u"Assets/effect/thunder.efk"},    // thunder
    };
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
            // 死亡ステート。
            RegisterState<app::enemyState::BossDeadState>(app::enemyStatus::state_Dead);


            // アニメーションをセットする。
            SetAnimation();

            // リストに登録したエフェクトをまとめてセットする。
            for (const auto& res : resources)
            {
                // IDとパスをセット。
                EffectEngine::GetInstance()->ResistEffect(res.effectID, res.path);
            }

            // モデルをセットする。
            render_.Init("Assets/modelData/enemy/boss.tkm", animClips_, app::enemyStatus::BossAnimation::bossAnim_Num, enModelUpAxisZ);
            render_.SetPosition(pos_);
            render_.SetScale(SCALE);


            rot_.AddRotationDegY(-90.0f);
            render_.SetRotation(rot_);
            pPlayer_ = FindGO<Player>("player");
            pCutInView_ = FindGO<app::cutIn::CutInView>("CutInView");


            pWeeekPoint_ = NewGO<CollisionObject>(0);
            pWeeekPoint_->CreateSphere(pos_ + Vector3(0.0f, 300.0f, 50.0f), Quaternion::Identity, WEEKE_POINT_RADIUS);
            pWeeekPoint_->SetIsEnableAutoDelete(false);
            pWeeekPoint_->SetIsEnable(false);

            weakPointBoneID_ = render_.FindBoneID(L"mixamorig:Head");
            // デフォルト値を使用。
            SetWeakPointHeight(22.0f, false);

            // 初期ステートの登録。
            pCurrentState_ = pStateList_[app::enemyStatus::state_Idle];
            if (pCurrentState_)
                pCurrentState_->Enter();

            // フェーズマネージャの初期化。
            app::core::BattlePhaseManager::GetInstance()->Init();

            // InputManagerの次元反転フラグを無効化。
            app::core::InputManager::GetInstance()->SetDimensionFlipFlag(true);

            // HPを初期化。
            SetHP(MAX_HP);

            // ボーンの名前をデバッグ出力。
            for (int i = 0; i < render_.GetNumBones(); ++i)
            {
                // i番目のボーンを取得
                auto* bone = render_.GetBone(i);
                if (bone)
                {
                    // 名前を出力
                    OutputDebugStringW(bone->GetName());
                    OutputDebugStringW(L"\n");
                }
            }

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

            // カメラのClamp制限。
            AddClamp();

            // 回転。
            Rotaition();

            // モデル本体の更新。
            render_.SetRotation(rot_);
            render_.SetScale(SCALE);
            render_.SetPosition(pos_);
            render_.Update();

            // ボーン位置にコリジョンを合わせる。
            if (pWeeekPoint_)
                pWeeekPoint_->SetPosition(GetWeakPoint());

            // UIにHPの情報を通知。
            app::nsUI::BossUIManager::GetInstance().OnUpdateHP(static_cast<float>(GetHP()), MAX_HP);
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
            if (!pPlayer_ || !pPlayer_->GetCameraManager())
                return;

            auto currentMode = pPlayer_->GetCameraManager()->GetCurrentCameraMode();

            // 2DモードのときだけZ軸を固定し、X軸を制限する
            if (currentMode == CameraMode::mode2D)
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
             animClips_[app::enemyStatus::BossAnimation::bossAnim_AttackCast].SetLoopFlag(true);

             // 咆哮攻撃状態。
             animClips_[app::enemyStatus::BossAnimation::bossAnim_AttackRoar].Load("Assets/animData/boss/roar.tka");
             animClips_[app::enemyStatus::BossAnimation::bossAnim_AttackRoar].SetLoopFlag(true);

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

             // 死亡ステート。
             animClips_[app::enemyStatus::BossAnimation::bossAnim_Dead].Load("Assets/animData/boss/dead.tka");
             animClips_[app::enemyStatus::BossAnimation::bossAnim_Dead].SetLoopFlag(false);
        }


        Vector3 Boss::RandomStagePos()
        {
            RANGE; // 生成範囲。

            // 1. ランダムな角度 (0 ～ 360度)
            float angle = static_cast<float>(rand() % 360) * (3.14159265f / 180.0f);

            // 2. ランダムな距離 (0 ～ 半径)
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


        Vector3 Boss::GetWeakPoint() const
        {
            if (isManualOverride_)
                return pos_ + Vector3(0.0f, manualWeakHeight_, 0.0f);

            // ボーン追従モード
            else
            {
                // ボーンのワールド行列を取得
                const Matrix& mat = render_.GetBone(weakPointBoneID_)->GetWorldMatrix();

                Vector3 bonePos;
                bonePos.x = mat.m[3][0];
                bonePos.y = mat.m[3][1];
                bonePos.z = mat.m[3][2];

                // オフセットを加算して返す
                return bonePos + Vector3(0.0f, WEAK_POINT_BONE_OFFSET_Y, 0.0f);
            }
        }
    } 
}

