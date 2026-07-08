#include "stdafx.h"
#include "BossTumbleState.h"

#include "Src/Actor/Character/Enemy/Boss/IBossStrategy.h"


// キャラクター。
#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Player/Player.h"

// コア。
#include "Src/Core/BattlePhaseManager.h"
#include "Src/Core/SoundManager.h"

// UI。
#include "Src/UI/ButtonActionUI.h"

// gimmick。
#include "Src/Actor/Stage/Gimmick/BossGimmick/FloatingPlatform.h"
#include "Src/Presentation/UI/BossAttackHudHelper.h"

namespace
{
    // 疲労状態の継続時間
    const float TIRED_DURATION = 20.0f;

    // 無敵時間。
    const float INVINCIBLE_TIME = 1.0f;

    // 足場が上がり切るまでの時間。
    const auto SCAFFOLD_MOVE_DURATION = 2.5f;
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
            spawnSeTimer_ = 0.0f;
            delayTimer_ = 0.0f;
            canBeAttacked_ = false;

            pBoss_->SetWeakPointHeight(22.0f, false);

            // 弱点を有効化。
            if (pBoss_->pWeeekPoint_)
            {
                pBoss_->pWeeekPoint_->SetIsEnable(false);
            }


            // 足場を生成する。
            app::core::BattlePhaseManager::GetInstance()->ActivateScaffolding();

            // 足場を生成時のSEを再生。
            app::core::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_SpawnBlock, 2.0f);

            // Seを一度だけ再生するフラグをリセット。
            isSePlayed_ = true;

            /*
             * アイコンを描画するように通知。
             * 旧: BossUIManager::OnNotifyAttack(Tumbler)
             */
            nsApp::nsBossHud::NotifyAttack(pBoss_, app::enemyStatus::AttackType::Tumbler);
        }


        void BossTumbleState::Update()
        {
            float deltaTime = g_gameTime->GetFrameDeltaTime();
            timer_ += deltaTime;

            // 足場SEを制御。
            if (isSePlayed_)
            {
                spawnSeTimer_ += deltaTime;
                if (spawnSeTimer_ >= SCAFFOLD_MOVE_DURATION)
                {
                    app::core::SoundManager::GetInstance()->StopSE(GameSoundList_SE_SpawnBlock);
                    isSePlayed_ = false;
                }
            }


            // 倒れこみ処理を待つ処理。
            if (!canBeAttacked_) {
                delayTimer_ += deltaTime;
                if (delayTimer_ >= INVINCIBLE_TIME)
                    canBeAttacked_ = true;
            }
        }


        void BossTumbleState::Exit()
        {
            // ステートを抜ける際、必ず音を止めるように制限。
            if (isSePlayed_)
            {
                app::core::SoundManager::GetInstance()->StopSE(GameSoundList_SE_SpawnBlock);
                isSePlayed_ = false;
            }

            // 弱点を無効化。
            canBeAttacked_ = false;

            // 復帰時に足場を消す。
            app::core::BattlePhaseManager::GetInstance()->DeactivateScaffolding();

            pBoss_->SetWeakPointHeight(22.0f, false);

            /*
             * アイコンを消すように通知。
             * 旧: BossUIManager::OnNotifyAttack(None)
             */
            nsApp::nsBossHud::NotifyAttack(pBoss_, app::enemyStatus::AttackType::Num);
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

            // ■ 修正1: 判定の基準点を「物理コリジョンの中心」に合わせる
            // (以前の +1.2f という固定オフセットはなくします)
            Vector3 weakPointCenter = pBoss_->GetWeakPoint();
            Vector3 playerPos = pPlayer->GetPlayerPos();

            // 差分ベクトル
            Vector3 diff = playerPos - weakPointCenter;

            float distXZSq = diff.x * diff.x + diff.z * diff.z;
            bool isNearXZ = (distXZSq < 3.0f * 3.0f);

            bool isAboveY = (diff.y > -1.0f && diff.y < 4.0f);

            // 落下中の判定をとる
            bool isFalling = (pPlayer->GetMoveSpeed().y <= 0.0f);

            // 範囲内かつ、落下中（踏みつけ）ならヒット
            if (isNearXZ && isAboveY && isFalling)
                return true;

            return false;
        }


       void BossTumbleState::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
        {
            // ★修正：イベントでは当たり判定の有効/無効だけを切り替える
            // 高さ（targetHeight_）の変更処理は不要になるので削除します

            if (wcscmp(eventName, L"Down_Start") == 0)
            {
                if (pBoss_->pWeeekPoint_)
                {
                    pBoss_->pWeeekPoint_->SetIsEnable(true);
                    // targetHeight_ = 5.0f; // 削除
                }
            }
            else if (wcscmp(eventName, L"Down_End") == 0)
            {
                if (pBoss_->pWeeekPoint_)
                {
                    pBoss_->pWeeekPoint_->SetIsEnable(true);
                    // targetHeight_ = 3.0f; // 削除
                }
            }
            else if (wcscmp(eventName, L"Stand_End") == 0)
            {
                // targetHeight_ = 22.0f; // 削除
                if (pBoss_->pWeeekPoint_)
                {
                    pBoss_->pWeeekPoint_->SetIsEnable(false);
                }
            }
        }
    }
}
