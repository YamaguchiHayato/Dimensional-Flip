#include "stdafx.h"

#include "BossHubScreen.h"
#include "Src/Actor/Stage/StageSetup.h"
#include "Src/Presentation/UI/Components/BossAttackIndicatorHudComponent.h"
#include "Src/Presentation/UI/Components/BossStatusHudComponent.h"

namespace nsApp
{
    namespace nsUI
    {
        bool BossHudScreen::IsDrawBlocked() const
        {
            /* ボス戦中の Pause 画面表示中は描画をブロックする。 */
            return nsStage::StageSetup::ShouldKeepPlayerPaused();
        }


        void BossHudScreen::Update(float deltaTime)
        {
            /* データソースから各 Component へ値を委譲する。 */
            SyncFromDataSource();
            UIScreen::Update(deltaTime);
        }


        void BossHudScreen::Draw(RenderContext& rc)
        {
            /* 描画可能かチェック。 */
            if (!IsVisible())
                return;

            /* 描画をブロックする場合は描画しない。 */
            if (IsDrawBlocked())
                return;

            /* ワールド行列を取得。 */
            const Matrix& world = GetRootEntity().GetTransform().GetWorldMatrix();

            /* 各 Component の描画を行う。 */
            if (pStatusComponent_ != nullptr)
                pStatusComponent_->OnDraw(rc, world);

            /* 各 Component の描画を行う。 */
            if (pAttackComponent_ != nullptr)
                pAttackComponent_->OnDraw(rc, world);
        }


        void BossHudScreen::Build()
        {
            /* ルート Entity を生成。 */
            nsFramework::Entity& root = CreateRootEntity();

            /* ルート Entity の位置を設定。 */
            Vector3 rootPos{650.0f, -400.0f, 0.0f};
            root.GetTransform().SetLocalPosition(rootPos);

            /* 各 Component を追加。 */
            pStatusComponent_ = &root.AddComponent<BossStatusHudComponent>();
            pAttackComponent_ = &root.AddComponent<BossAttackIndicatorHudComponent>();
        }


        void BossHudScreen::Bind(nsPresentation::IBossHudData* pData)
        {
            /* データソースを設定。 */
            pHudData_ = pData;
            SyncFromDataSource();
        }


        void BossHudScreen::SyncFromDataSource()
        {
            /* データソースが nullptr の場合は何もしない。 */
            if (pHudData_ == nullptr)
                return;

            /* データソースから各 Component へ値を委譲する。 */
            if (pStatusComponent_ != nullptr)
                pStatusComponent_->SetHpPercent(pHudData_->GetHpRatio());
            if (pAttackComponent_ != nullptr)
                pAttackComponent_->SetAttackKind(pHudData_->GetAttackKind());
        }
    } // namespace nsUI
} // namespace nsApp
