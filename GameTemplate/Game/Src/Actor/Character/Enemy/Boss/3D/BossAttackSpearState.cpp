#include "stdafx.h"

#include "BossAttackSpearState.h"
#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Stage/Gimmick/BossGimmick/Spear.h"

#include "Src/Core/BossUIManager.h"
#include "Src/Core/SoundManager.h"

namespace
{
    const float SPAWN_TIME = 0.5f;

    // 壁の生成範囲
    const float STAGE_WIDTH_HALF = 35.0f; // 横幅の半分
    const float WALL_HEIGHT = 25.0f;      // 壁の高さ

    // 槍の間隔
    const float GRID_SIZE = 5.0f;

    // 出現位置（Z軸の奥と手前）
    const float SPAWN_Z_DIST = 80.0f;

    const float SAFE_GAP_Z = 7.0f;
} 

namespace app
{
    namespace enemyState
    {
        void BossAttackSpearState::Enter(app::enemy::Boss* pBoss)
        {
            pBoss_ = pBoss;
            timer_ = 0.0f;
            isAttackSpawned_ = false;

            // アニメーションの再生。
            pBoss_->LoadAnimation(app::enemyStatus::BossAnimation::bossAnim_AttackCast, true, 0.1f);

            // 攻撃のタイプをセット。
            pBoss_->SetAttackType(app::enemyStatus::AttackType::Spear);

            // 攻撃UIをセット。
            app::nsUI::BossUIManager::GetInstance().OnNotifyAttack(app::nsUI::BossAttackKind::Spear);
        }


        void BossAttackSpearState::Update()
        {
            timer_ += g_gameTime->GetFrameDeltaTime();

            if (timer_ >= SPAWN_TIME && !isAttackSpawned_)
            {
                isAttackSpawned_ = true;
                CreateSpearAttack();
            }
        }


        void BossAttackSpearState::Exit()
        {
            // 攻撃のインターバル時間をセット。
            pBoss_->SettNextInterval(3.0f);

            // 攻撃UIをセット。
            app::nsUI::BossUIManager::GetInstance().OnNotifyAttack(app::nsUI::BossAttackKind::Spear);
        }


        void BossAttackSpearState::CreateSpearAttack()
        {
            // SEの再生。
            app::core::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_Spear, 2.0);

            centerX_ = 0.0f;
            if (auto* pPlayer = pBoss_->GetPlayer())
                centerX_ = pPlayer->GetPlayerPos().x;

            randomOffset_ = (static_cast<float>(rand() % 200) / 10.0f) - 10.0f;
            centerX_ += randomOffset_;

            startX_ = centerX_ - 25.0f;
            endX_ = centerX_ + 25.0f;
            pattern_ = rand() % 3;

            // グリッド状に生成
            for (float x = startX_; x <= endX_; x += GRID_SIZE)
            {
                // ステージ外ならスキップ
                if (x < -STAGE_WIDTH_HALF || x > STAGE_WIDTH_HALF)
                    continue;

                for (float y = 2.0f; y <= WALL_HEIGHT; y += GRID_SIZE)
                {
                    // --- パターン判定: 奥の壁を出すか？ ---
                    if (pattern_ == 0 || pattern_ == 2)
                    {
                        // 奥の壁 (Z+ -> 手前へ)
                        auto spearBack = NewGO<app::gimmick::Spear>(0);
                        spearBack->SetType(SpearType::Cross);

                        posBackStart_ = Vector3(x, y, SPAWN_Z_DIST);
                        posBackTarget_ = Vector3(x, y, SAFE_GAP_Z); // +7.0fで止まる

                        spearBack->SetStartPos(posBackStart_);
                        spearBack->SetTargetPos(posBackTarget_);
                    }

                    // --- パターン判定: 手前の壁を出すか？ ---
                    if (pattern_ == 1 || pattern_ == 2)
                    {
                        // 手前の壁 (Z- -> 奥へ)
                        auto spearFront = NewGO<app::gimmick::Spear>(0);
                        spearFront->SetType(SpearType::Cross);

                        posFrontStart_ = Vector3(x, y, -SPAWN_Z_DIST);
                        posFrontTarget_ = Vector3(x, y, -SAFE_GAP_Z); // -7.0fで止まる

                        spearFront->SetStartPos(posFrontStart_);
                        spearFront->SetTargetPos(posFrontTarget_);
                    }
                }
            }
        }
    } // namespace enemyState
} // namespace app
