#include "stdafx.h"

#include "BossAttackIndicatorHudComponent.h"
#include "Src/Presentation/UI/BossHudUiNames.h"

namespace
{
    const float TEX_W = 1920.0f;
    const float TEX_H = 1080.0f;
    const float MAGNIFICATION = 0.25f;
    const Vector3 ATTACK_ICON_OFFSET(-650.0f, -25.0f, 0.0f);
    const Vector3 ATTACK_ICON_POS(0.0f, -425.0f, 0.0f);

    struct IconDef
    {
        nsApp::nsUI::BossAttackKind kind;
        const char* uiName;
    };

    const IconDef ICON_DEFS[] = {
        {nsApp::nsUI::BossAttackKind::FireBall, "fireBall"}, {nsApp::nsUI::BossAttackKind::Jump, "jumpIcon"},
        {nsApp::nsUI::BossAttackKind::Meteor, "meteorIcon"}, {nsApp::nsUI::BossAttackKind::Roar, "roarIcon"},
        {nsApp::nsUI::BossAttackKind::Spear, "spearIcon"},   {nsApp::nsUI::BossAttackKind::Tumbler, "tumblerIcon"},
    };
} // namespace


namespace nsApp
{
    namespace nsUI
    {
        void BossAttackIndicatorHudComponent::SetAttackKind(BossAttackKind kind)
        {
            currentKind_ = kind;
        }


        void BossAttackIndicatorHudComponent::OnBuild()
        {
            /* 攻撃アイコンを一括初期化する。 */
            for (const auto& def : ICON_DEFS)
            {
                const int index = static_cast<int>(def.kind);
                attackIcons_[index].Init(FetchUIName(def.uiName).c_str(), TEX_W, TEX_H);
                attackIcons_[index].SetPivot({0.5f, 0.5f});
                attackIcons_[index].SetScale(Vector3(MAGNIFICATION, MAGNIFICATION, 1.0f));
            }

            isBuilt_ = true;
        }


        void BossAttackIndicatorHudComponent::OnUpdate(float deltaTime)
        {
            (void) deltaTime;
        }


        void BossAttackIndicatorHudComponent::OnDraw(RenderContext& rc, const Matrix& ownerWorld)
        {
            (void) ownerWorld;

            if (!isBuilt_)
                return;

            if (currentKind_ == BossAttackKind::None)
                return;

            const int index = static_cast<int>(currentKind_);
            if (index < 0 || index >= static_cast<int>(BossAttackKind::Num))
                return;

            attackIcons_[index].SetPosition(ATTACK_ICON_POS);
            attackIcons_[index].Update();
            attackIcons_[index].Draw(rc);
        }
    } // namespace nsUI
} // namespace nsApp
