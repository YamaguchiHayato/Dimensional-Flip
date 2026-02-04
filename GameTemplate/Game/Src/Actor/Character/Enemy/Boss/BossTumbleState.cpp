#include "stdafx.h"
#include "BossTumbleState.h"

#include "Src/Actor/Character/Enemy/Boss/IBossStrategy.h"


// キャラクター。
#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Player/Player.h"

// コア。
#include "Src/Core/BattlePhaseManager.h"
#include "Src/Core/BossUIManager.h"
#include "Src/Core/SoundManager.h"

// UI。
#include "Src/UI/ButtonActionUI.h"

// gimmick。
#include "Src/Actor/Stage/Gimmick/BossGimmick/FloatingPlatform.h"

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
            // アニメーションイベントをコールバック関数として登録。
            pBoss_->render_.AddAnimationEvent([this](const wchar_t* clip, const wchar_t* event){this->OnAnimationEvent(clip, event);});

            // 疲労アニメーションを再生させる。
            pBoss_->LoadAnimation(app::enemyStatus::BossAnimation::bossAnim_Tumble, true, 0.1f);

            // タイマーをリセット。
            timer_ = 0.0f;
            delayTimer_ = 0.0f;
            canBeAttacked_ = false;
            currentWeakHeight_ = 22.0f;
            pBoss_->SetWeakPointHeight(currentWeakHeight_, true);

            // 弱点を有効化。
            if (pBoss_->pWeeekPoint_)
            {
                pBoss_->pWeeekPoint_->SetIsEnable(false);
            }



            // アイコンを描画するように通知。
            app::nsUI::BossUIManager::GetInstance().OnNotifyAttack(app::nsUI::BossAttackKind::Tumbler);


            // 足場を生成する。
            app::core::BattlePhaseManager::GetInstance()->ActivateScaffolding();
        }


        void BossTumbleState::Update()
        {
            float deltaTime = g_gameTime->GetFrameDeltaTime();
            timer_ += deltaTime;

            currentWeakHeight_ += (targetHeight_ - currentWeakHeight_) * 0.1f;

            // 現在のボスの高さをボスに通達。
            pBoss_->SetWeakPointHeight(currentWeakHeight_, true);

            // 倒れこみ処理を待つ処理。
            if (!canBeAttacked_) {
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

            pBoss_->SetWeakPointHeight(22.0f, false);
        }


        bool BossTumbleState::RequestID(uint8_t& request)
        {
            // 弱点攻撃判定。
            if (canBeAttacked_ )
            {
                if (CheckWeakPointHit())
                {
                    // Playerのバウンド処理。
                    if (auto* pPlayer = pBoss_->GetPlayer())
                        pPlayer->Bound();

                    // ヒットSEを鳴らす。
                    app::core::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_BossHit);

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
            headPos_ = pBoss_->GetWeakPoint() + Vector3(0.0f, 1.2f, 0.0f);
            playerPos_ = pPlayer->GetPlayerPos();


            // 距離判定用ベクトルの作成。
            diff_ = playerPos_ - headPos_;
            auto distXZ = sqrtf(diff_.x * diff_.x + diff_.z * diff_.z);

            // 水平方向。
            bool isNearXZ_ = (distXZ < 1.2f);

            // 高さ判定。
            bool isAboveY = (diff_.y > -0.5f && diff_.y < 0.4f);

            // 落下中の判定をとる
            bool isFalling = (pPlayer->GetMoveSpeed().y <= 0.0f);


            // 範囲内かつ、ボスの頭上付近にいればヒット判定とする。
            if (isNearXZ_ && isAboveY && isFalling)
                return true;

            return false;
        }


        void BossTumbleState::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
        {
            if (wcscmp(eventName, L"Down_Start") == 0)
            {
                if (pBoss_->pWeeekPoint_)
                {
                    pBoss_->pWeeekPoint_->SetIsEnable(true);
                    targetHeight_ = 5.0f; // 倒れこみ開始時の低い値。
                }
            }
            else if (wcscmp(eventName, L"Down_End") == 0)
            {
                if (pBoss_->pWeeekPoint_)
                {
                    pBoss_->pWeeekPoint_->SetIsEnable(true);
                    targetHeight_ = 3.0f; // 完全に倒れこんだ値。
                }
            }

            // 起き上がり
            if (wcscmp(eventName, L"Stand_Start") == 0)
            {
                targetHeight_ = 8.0f; // 地面付近の高さ
            }
            else if (wcscmp(eventName, L"Stand_End") == 0)
            {
                targetHeight_ = 22.0f; // 中間の高さ

                if (pBoss_->pWeeekPoint_)
                {
                    pBoss_->pWeeekPoint_->SetIsEnable(false);
                }
            }
        }
    }
}
