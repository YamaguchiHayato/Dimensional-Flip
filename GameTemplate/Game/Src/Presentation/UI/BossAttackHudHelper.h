#pragma once

#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Enemy/Boss/BossType.h"
#include "Src/Core/Game.h"
#include "Src/Presentation/Data/BossHubData.h"
#include "k2EnginePreCompile.h"

namespace nsApp
{
    namespace nsBossHud
    {
        /**
         * @brief 旧 BossUIManager::OnNotifyAttack の代替。
         *        SetAttackType → BossHudData::SyncToScreen で即時反映する。
         * @param pBoss ボス本体。
         * @param type  攻撃種別。Num でアイコン非表示。
         */
        inline void NotifyAttack(app::enemy::Boss* pBoss, app::enemyStatus::AttackType type)
        {
            /* ボスが nullptr の場合は処理をしない。 */
            if (pBoss == nullptr)
                return;

            /* Boss が保持する攻撃種別を更新（BossHudData がここを読む）。 */
            pBoss->SetAttackType(type);

            /* 旧 OnNotifyAttack と同様、同フレームで Screen へ反映。 */
            if (auto* pGame = FindGO<nsApp::nsCore::Game>("game"))
            {
                if (auto* pData = pGame->GetBossHudData())
                {
                    /* Bossクラスが参照できるように。*/
                    pData->SetBoss(pBoss);
                    pData->SyncToScreen();
                }
            }
        }
    } // namespace nsBossHud
} // namespace nsApp
