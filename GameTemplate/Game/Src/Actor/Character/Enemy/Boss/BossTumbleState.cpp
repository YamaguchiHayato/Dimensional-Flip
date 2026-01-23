#include "stdafx.h"
#include "BossTumbleState.h"

#include "Src/Actor/Character/Enemy/Boss/IBossStrategy.h"


// キャラクター。
#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Player/Player.h"

// コア。
#include "Src/Core/BattlePhaseManager.h"

#include "Src/Actor/Stage/Gimmick/FloatingPlatform.h"

namespace
{
    // 疲労状態の継続時間
    const float TIRED_DURATION = 20.0f;
}


namespace app
{
    namespace enemyState
    {
        void BossTumbleState::Enter()
        {
            // 疲労アニメーションを再生させる。
            pBoss_->LoadAnimation(app::enemyStatus::BossAnimation::bossAnim_Tumble, false, 0.1f);

            // タイマーをリセット。
            timer_ = 0.0f;

            // 弱点を有効化。
            if (pBoss_->pWeeekPoint_)
                pBoss_->pWeeekPoint_->SetIsEnable(true);

            canBeAttacked_ = true;


            // 足場を生成する。
            app::core::BattlePhaseManager::GetInstance()->ActivateScaffolding();
        }


        void BossTumbleState::Update()
        {
            timer_ += g_gameTime->GetFrameDeltaTime();
        }


        void BossTumbleState::Exit()
        {
            // 弱点を無効化。
            canBeAttacked_ = false;

            // 復帰時に足場を消す。
            app::core::BattlePhaseManager::GetInstance()->DeactivateScaffolding();
        }

        bool BossTumbleState::RequestID(uint8_t& request)
        {
            // 弱点攻撃判定。
            if (canBeAttacked_ && CheckWeakPointHit())
            {
                request = app::enemyStatus::BossState::state_Hit;
                return true;
            }

            // 時間切れ判定。
            if (timer_ >= TIRED_DURATION)
            {
                // 待機状態へ戻る。
                pBoss_->SettNextInterval(3.0f);
                request = app::enemyStatus::BossState::state_Idle;
                return true;
            }

            return false;
        }


        bool BossTumbleState::CheckWeakPointHit()
        {
            // Playerを取得する。
            Player* pPlayer = pBoss_->GetPlayer();

            // ボスへの攻撃判定ベクトルの作成。
            Vector3 headPos = pBoss_->GetPos() + Vector3(0.0f, 150.0f, 0.0f);
            Vector3 playerPos = pPlayer->GetPlayerPos();

            // 距離判定用ベクトルの作成。
            Vector3 diff = playerPos - headPos;
            auto distXZ = sqrtf(diff.x * diff.x + diff.z * diff.z);
            bool isAbove = (diff.y >-50.0f);

            // 範囲内かつ、ボスの頭上付近にいればヒット判定とする。
            if (distXZ < 200.0f && isAbove)
                return true;

            return false;

        }
    }
}
