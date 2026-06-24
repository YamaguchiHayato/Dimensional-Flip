#include "stdafx.h"
#include "BossDeadState.h"

#include "Src/Production/Fade.h"
#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Core/SceneManager.h"
#include "Src/Core/SoundManager.h"

namespace app
{
    namespace enemyState
    {
        void BossDeadState::Enter()
        {
            // 死亡アニメーションを再生する。
            pBoss_->LoadAnimation(app::enemyStatus::BossAnimation::bossAnim_Dead, false, 0.2f);

            if (pBoss_->pWeeekPoint_)
                // 攻撃ポイントを無効化する。
                pBoss_->pWeeekPoint_->SetIsEnable(false);

            // ボス戦のBGMを停止する。
            app::core::SoundManager::GetInstance()->StopBGM(GameSoundList_BGM_BOSS);

            pFade_ = SceneManager::GetInstance()->GetFade();
        }


        void BossDeadState::Update()
        {
            if (pBoss_->IsPlayingAnimation())
                return;

            auto* pGoalFade = SceneManager::GetInstance()->GetFade();

            if (!isFadeStarted_)
            {
                if (pGoalFade)
                    pGoalFade->StartFadeOut();

                isFadeStarted_ = true;
            }

            // フェードを確認する処理。
            if (pFade_ && pFade_->IsFadeOutEnd())
                SceneManager::GetInstance()->ChangeScene(SceneID::sEndRoll);
        }


        void BossDeadState::Exit()
        {

        }


        bool BossDeadState::RequestID(uint8_t& request)
        {
            return false;
        }
    }
}
