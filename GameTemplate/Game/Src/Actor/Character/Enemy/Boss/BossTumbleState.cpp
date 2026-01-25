#include "stdafx.h"
#include "BossTumbleState.h"

#include "Src/Actor/Character/Enemy/Boss/IBossStrategy.h"


// キャラクター。
#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Player/Player.h"

// コア。
#include "Src/Core/BattlePhaseManager.h"
#include "Src/Core/BossUIManager.h"


// UI。
#include "Src/UI/ButtonActionUI.h"

// gimmick。
#include "Src/Actor/Stage/Gimmick/FloatingPlatform.h"

namespace
{
    // 疲労状態の継続時間
    const float TIRED_DURATION = 20.0f;

    // 無敵時間。
    const float INVINCIBLE_TIME = 1.0f;
}


namespace app
{
    namespace enemyState
    {
        void BossTumbleState::Enter()
        {
            // 疲労アニメーションを再生させる。
            pBoss_->LoadAnimation(app::enemyStatus::BossAnimation::bossAnim_Tumble, true, 0.1f);

            // タイマーをリセット。
            timer_ = 0.0f;
            delayTimer_ = 0.0f;

            canBeAttacked_ = false;

            // 弱点を有効化。
            if (pBoss_->pWeeekPoint_)
                pBoss_->pWeeekPoint_->SetIsEnable(true);

            // アイコンを描画するように通知。
            app::nsUI::BossUIManager::GetInstance().OnNotifyAttack(app::nsUI::BossAttackKind::Tumbler);


            // 足場を生成する。
            app::core::BattlePhaseManager::GetInstance()->ActivateScaffolding();
        }


        void BossTumbleState::Update()
        {
            float deltaTime = g_gameTime->GetFrameDeltaTime();
            timer_ += deltaTime;

            // 倒れこみ処理を待つ処理。
            if (!canBeAttacked_)
            {
                delayTimer_ += deltaTime;


                if (delayTimer_ >= INVINCIBLE_TIME)
                    canBeAttacked_ = true;
            }
        }


        void BossTumbleState::Exit()
        {
            // 弱点を無効化。
            canBeAttacked_ = false;

            // 復帰時に足場を消す。
            app::core::BattlePhaseManager::GetInstance()->DeactivateScaffolding();

            // アイコンを消すように通知。
            app::nsUI::BossUIManager::GetInstance().OnNotifyAttack(app::nsUI::BossAttackKind::None);
        }


        bool BossTumbleState::RequestID(uint8_t& request)
        {
            // 弱点攻撃判定。
            if (canBeAttacked_ )
            {
                if (CheckWeakPointHit())
                {
                    request = app::enemyStatus::BossState::state_Hit;
                    return true;
                }
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
            if (!pPlayer)
                return false;

            // ボスへの攻撃判定ベクトルの作成。
            headPos_ = pBoss_->GetPos() + Vector3(0.0f, 22.0f, 0.0f);
            playerPos_ = pPlayer->GetPlayerPos();


            // 距離判定用ベクトルの作成。
            diff_ = playerPos_ - headPos_;
            auto distXZ = sqrtf(diff_.x * diff_.x + diff_.z * diff_.z);


            // 高さを指定する。
            isAbove_ = (diff_.y > 0.0f && diff_.y < 30.0f);

            // 範囲内かつ、ボスの頭上付近にいればヒット判定とする。
            if (distXZ < 5.0f && isAbove_)
            {
                // @TODO:  ヒット時の効果音の再生を行う。
                return true;
            }

            return false;

        }
    }
}
