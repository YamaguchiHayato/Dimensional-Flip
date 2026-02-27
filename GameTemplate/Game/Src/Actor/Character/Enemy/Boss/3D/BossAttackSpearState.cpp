#include "stdafx.h"

#include "BossAttackSpearState.h"
#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Stage/Gimmick/BossGimmick/Spear.h"

namespace
{
    const float SPAWN_TIME = 0.5f;

    // 壁の生成範囲
    const float STAGE_WIDTH_HALF = 35.0f; // 横幅の半分
    const float WALL_HEIGHT = 25.0f;      // 壁の高さ

    // 槍の間隔（グリッドサイズ）
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

            pBoss_->LoadAnimation(app::enemyStatus::BossAnimation::bossAnim_AttackCast, true, 0.1f);
            pBoss_->SetAttackType(app::enemyStatus::AttackType::Spear);
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
            pBoss_->SettNextInterval(3.0f);
        }


        void BossAttackSpearState::CreateSpearAttack()
        {
            float centerX = 0.0f;
            if (auto* pPlayer = pBoss_->GetPlayer())
                centerX = pPlayer->GetPlayerPos().x;

            float randomOffset = (static_cast<float>(rand() % 200) / 10.0f) - 10.0f;
            centerX += randomOffset;

            float startX = centerX - 25.0f;
            float endX = centerX + 25.0f;
            int pattern = rand() % 3;

            // グリッド状に生成
            for (float x = startX; x <= endX; x += GRID_SIZE)
            {
                // ステージ外ならスキップ
                if (x < -STAGE_WIDTH_HALF || x > STAGE_WIDTH_HALF)
                    continue;

                for (float y = 2.0f; y <= WALL_HEIGHT; y += GRID_SIZE)
                {
                    // --- パターン判定: 奥の壁を出すか？ ---
                    if (pattern == 0 || pattern == 2)
                    {
                        // 奥の壁 (Z+ -> 手前へ)
                        auto spearBack = NewGO<app::gimmick::Spear>(0);
                        spearBack->SetType(SpearType::Cross);

                        Vector3 posBackStart(x, y, SPAWN_Z_DIST);
                        Vector3 posBackTarget(x, y, SAFE_GAP_Z); // +7.0fで止まる

                        spearBack->SetStartPos(posBackStart);
                        spearBack->SetTargetPos(posBackTarget);
                    }

                    // --- パターン判定: 手前の壁を出すか？ ---
                    if (pattern == 1 || pattern == 2)
                    {
                        // 手前の壁 (Z- -> 奥へ)
                        auto spearFront = NewGO<app::gimmick::Spear>(0);
                        spearFront->SetType(SpearType::Cross);

                        Vector3 posFrontStart(x, y, -SPAWN_Z_DIST);
                        Vector3 posFrontTarget(x, y, -SAFE_GAP_Z); // -7.0fで止まる

                        spearFront->SetStartPos(posFrontStart);
                        spearFront->SetTargetPos(posFrontTarget);
                    }
                }
            }
        }
    } // namespace enemyState
} // namespace app
