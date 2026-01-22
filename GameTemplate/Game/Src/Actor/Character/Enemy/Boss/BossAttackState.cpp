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
#include "Src/Actor/Character/Enemy/Boss/2D/BossAttackFireBallState.h"
#include "Src/Actor/Character/Enemy/Boss/2D/BossAttackRoar2DState.h"

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
            // カメラモードを取得。
            CameraMode mode = CameraMode::mode3D;
            if (auto* pPlayer = pBoss_->GetPlayer())
            {
                if (auto* pCamMan = pPlayer->GetCameraManager())
                    mode = pCamMan->GetCurrentCameraMode();
            }

            // モードに応じた戦略の抽選
            if (mode == CameraMode::mode3D)
                // 3D視点の攻撃方法。
                DecideStrategy3D();

            else
                // 2D視点の攻撃方法。
                DecideStrategy2D();

            // 攻撃ステートの開始処理。
            if (currentState_)
                currentState_->Enter(pBoss_);
        }

        void BossAttackState::Update()
        {
            // 攻撃ステートの更新。
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
                    // 攻撃回数リセット
                    pBoss_->ResetAttackCount();

                    // 倒れ状態へ移行
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

            // 隕石攻撃ステート。
            case app::enemyStatus::Attack3DType::type_Meteor:
                currentState_ = std::make_unique<BossAttackMeteoState>();
                break;

            // 槍攻撃ステート。
            case app::enemyStatus::Attack3DType::type_Spear:
                currentState_ = std::make_unique<BossAttackSpearState>();
                break;

            // 咆哮攻撃ステート。
            case app::enemyStatus::Attack3DType::type_3DRoar:
                currentState_ = std::make_unique<BossAttackRoar3DState>();
                break;

            default:
                break;
            }
        }

        void BossAttackState::DecideStrategy2D()
        {
            int typeNum = static_cast<int>(app::enemyStatus::Attack2DType::type_Num);
            int rand2DNum = 0;

            // 被り防止のため、抽選し直す。
            if (typeNum > 1)
            {
                do
                {
                    rand2DNum = rand() % typeNum;
                } while (rand2DNum == lastAttackType_);
            }
            else
            {
                rand2DNum = 0;
            }

            // 今回の攻撃タイプを保存。
            lastAttackType_ = rand2DNum;

            // キャスト処理。
            auto attackType = static_cast<app::enemyStatus::Attack2DType>(rand2DNum);

            switch (attackType)
            {
            case app::enemyStatus::Attack2DType::type_Jump:
                currentState_ = std::make_unique<BossAttackJumpState>();
                break;

            case app::enemyStatus::Attack2DType::type_FireBall:
                currentState_ = std::make_unique<BossAttackFireBallState>();
                break;

            case app::enemyStatus::Attack2DType::type_2DRoar:
                currentState_ = std::make_unique<BossAttackRoar2DState>();
                break;

            default:
                break;
            }
        }
    } 
} 
