#include "stdafx.h"
#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Player/Player.h"

// ボス戦時のギミッククラス。
#include "Src/Actor/Stage/Gimmick/Meteo.h"
#include "Src/Actor/Stage/Gimmick/Spear.h"
#include "Src/Actor/Stage/Gimmick/FloatingPlatform.h"


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
            for (uint8_t i = 0; i < BossState::state_Num; ++i)
            {
                if (pStateList_[i])
                {
                    delete pStateList_[i];
                    pStateList_[i] = nullptr;
                }
            }
        }


        bool Boss::Start()
        {
            // ステートの登録。
            // 待機ステート。
            RegisterState<BossIdleState>(state_Idle);
            // 攻撃ステート。
            RegisterState<BossAttackState>(state_Attack);
            // 疲労ステート。
            RegisterState<BossTumbleState>(state_Tumble);
            // ダメージステート。
            RegisterState<BossDamageState>(state_Hit);

            // アニメーションをセットする。
            SetAnimation();
            // モデルをセットする。
            render_.Init("Assets/modelData/enemy/boss.tkm", ANIMCLIP, BossAnimation::bossAnim_Num, enModelUpAxisZ);
            render_.SetPosition(pos_);

            rot_.AddRotationDegY(270.0f);
            render_.SetRotation(rot_);
            pPlayer_ = FindGO<Player>("player");

            pWeeekPoint_ = NewGO<CollisionObject>(0);

            pWeeekPoint_->CreateSphere(pos_ + Vector3(0.0f, 300.0f, 50.0f), Quaternion::Identity, WEEKE_POINT_RADIUS);
            pWeeekPoint_->SetIsEnableAutoDelete(false);
            pWeeekPoint_->SetIsEnable(false);


            // 初期ステートの登録。
            pCurrentState_ = pStateList_[state_Idle];
            if (pCurrentState_)
                pCurrentState_->Enter();

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


            // 回転。
            Rotaition();

            // 攻撃可能をコリジョンの位置。   
            if (pWeeekPoint_)
                pWeeekPoint_->SetPosition(pos_ + Vector3(0.0f, 200.0f, 0.0f));

            // モデル本体の更新。
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
                ANIMCLIP[BossAnimation::bossAnim_Tumble].SetLoopFlag(true);

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
    }
}

