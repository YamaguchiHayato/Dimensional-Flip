#include "stdafx.h"
#include "BossAttackState.h"


// プレイヤーの情報を取得。
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Core/CameraManager.h"

#include "Src/Actor/Character/Enemy/Boss/BossType.h"
#include "Src/Actor/Character/Enemy/Boss/Boss.h"


// 攻撃の種類。
// 3D。
#include "Src/Actor/Character/Enemy/Boss/3D/BossAttackMeteoState.h"
#include "Src/Actor/Character/Enemy/Boss/3D/BossAttackSpearState.h"
#include "Src/Actor/Character/Enemy/Boss/3D/BossAttackRoar3DState.h"

// 2D。
#include "Src/Actor/Character/Enemy/Boss/2D/BossAttackJumpState.h"

namespace
{
    const auto ATTACK_SPAWN_TIME = 0.8f; // 何秒で攻撃オブジェクトを生成し始めるか。

    // 攻撃種類ごとのインターバル時間。
    // Cast。
    const auto ATTACK_INTERVAL_ROAR = 4.0f + (static_cast<float>(rand() % 21) / 10.0f);
    // Roar。
    const auto ATTACK_INTERVAL_CAST = 2.0f + (static_cast<float>(rand() % 16) / 10.0f);


    // ランダムなインターバル時間を生成する。
    // Cast攻撃用。
    float GetRandomCastInterval() {
        return ATTACK_INTERVAL_CAST;
    }
    
    // Roar攻撃用。
    float GetRandomRoarInterval() {
        return ATTACK_INTERVAL_ROAR;
    }
}

namespace app
{
    namespace enemyState
    {
        void BossAttackState::Enter()
        {
            CameraMode mode = CameraMode::mode3D;

            // Player -> CameraManager -> GetCurrentCameraMode() の順にたどって取得
            if (auto* pPlayer = pBoss_->GetPlayer())
            {
                if (auto* pCamMan = pPlayer->GetCameraManager())
                    mode = pCamMan->GetCurrentCameraMode();
            }

            // 2. モードに応じた戦略の抽選
            if (mode == CameraMode::mode3D)
                DecideStrategy3D();

            else
                DecideStrategy2D();

            if (currentState_)
                currentState_->Enter(pBoss_);
        }


        void BossAttackState::Update()
        {
            if (currentState_)            
                currentState_->Update();
        }


        void BossAttackState::Exit()
        {
            // 終了処理
            if (currentState_)
            {
                currentState_->Exit();
                currentState_.reset(); // メモリ解放
            }
        }


        bool BossAttackState::RequestID(uint8_t& request)
        {
            if (currentState_ && currentState_->IsFinished())
            {
                // 終わっていれば後始末 (ここでインターバル設定などが実行される)
                currentState_->Exit();

                // 攻撃回数カウントなどの共通処理
                pBoss_->AddAttackCount();


                // 疲労判定
                if (pBoss_->IsTired())
                {
                    pBoss_->ResetAttackCount();
                    request = app::enemyStatus::BossState::state_Tumble;
                    return true;
                }
                else
                {
                    // 待機へ戻る
                    request = app::enemyStatus::BossState::state_Idle;
                    return true;
                }
            }
            return false;
        }


        void BossAttackState::DecideStrategy3D()
        {
            // 攻撃は3種類の中からランダムで抽選。
            int rand3DNum = rand() % static_cast<int>(app::enemyStatus::Attack3DType::type_Num);

            // キャスト処理。
            auto attackType = static_cast<app::enemyStatus::Attack3DType>(rand3DNum);

            switch (attackType)
            {
            case app::enemyStatus::Attack3DType::type_Meteor:
                currentState_ = std::make_unique<BossAttackMeteoState>();
                break;

            case app::enemyStatus::Attack3DType::type_Spear:
                // 槍攻撃ステートをセット。
                currentState_ = std::make_unique<BossAttackSpearState>();
                break;

            case app::enemyStatus::Attack3DType::type_Roar:
                // 咆哮攻撃ステートをセット。
                currentState_ = std::make_unique<BossAttackRoar3DState>();
                break;

            default:
                break;
            }
        }


        void BossAttackState::DecideStrategy2D()
        {
            currentState_ = std::make_unique<BossAttackJumpState>();
        }
    }
}
