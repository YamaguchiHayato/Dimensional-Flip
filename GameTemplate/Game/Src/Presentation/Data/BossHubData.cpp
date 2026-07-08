#include "stdafx.h"

#include "BossHubData.h"
#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Enemy/Boss/BossType.h"
#include "Src/Presentation/UI/Screens/BossHubScreen.h"

namespace
{
    /** @brief Boss.cpp の MAX_HP と合わせる。 */
    const float BOSS_MAX_HP = 3.0f;

    /**
     * @brief ボス攻撃タイプを HUD 用アイコン種別へ変換する。
     * @param type ボス攻撃タイプ。
     * @return 攻撃アイコン種別。
     */
    nsApp::nsUI::BossAttackKind ToAttackKind(app::enemyStatus::AttackType type)
    {
        using AT = app::enemyStatus::AttackType;

        switch (type)
        {
        case AT::FireBall:
            return nsApp::nsUI::BossAttackKind::FireBall;
        case AT::Jump:
            return nsApp::nsUI::BossAttackKind::Jump;
        case AT::Meteor:
            return nsApp::nsUI::BossAttackKind::Meteor;
        case AT::Spear:
            return nsApp::nsUI::BossAttackKind::Spear;
        case AT::Roar3D:
        case AT::Roar2D:
            return nsApp::nsUI::BossAttackKind::Roar;
        case AT::Tumbler:
            return nsApp::nsUI::BossAttackKind::Tumbler;
        default:
            return nsApp::nsUI::BossAttackKind::None;
        }
    }
} // namespace

namespace nsApp
{
    namespace nsPresentation
    {
        void BossHudData::SetScreen(nsUI::BossHudScreen* pScreen)
        {
            /* Screen を登録し、即時同期する。 */
            pScreen_ = pScreen;
            SyncToScreen();
        }


        void BossHudData::SetBoss(app::enemy::Boss* pBoss)
        {
            /* 監視対象ボスを登録し、即時同期する。 */
            pBoss_ = pBoss;
            SyncToScreen();
        }


        float BossHudData::GetCurrentHp() const
        {
            /* Boss から HP を読む。 */
            if (pBoss_ == nullptr)
                return 0.0f;

            return static_cast<float>(pBoss_->GetHP());
        }


        float BossHudData::GetMaxHp() const
        {
            return BOSS_MAX_HP;
        }


        float BossHudData::GetHpRatio() const
        {
            const float maxHp = GetMaxHp();
            if (maxHp <= 0.0f)
                return 0.0f;

            return GetCurrentHp() / maxHp;
        }


        nsUI::BossAttackKind BossHudData::GetAttackKind() const
        {
            /* Boss の攻撃タイプを HUD 用に変換して返す。 */
            if (pBoss_ == nullptr)
                return nsUI::BossAttackKind::None;

            return ToAttackKind(pBoss_->GetAttackType());
        }


        void BossHudData::SyncToScreen()
        {
            /* Screen へ委譲する。 */
            if (pScreen_ != nullptr)
                pScreen_->SyncFromDataSource();
        }
    } // namespace nsPresentation
} // namespace nsApp
