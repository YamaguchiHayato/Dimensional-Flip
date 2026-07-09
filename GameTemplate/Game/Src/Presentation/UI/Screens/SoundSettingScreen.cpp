#include "stdafx.h"

#include "SoundSettingScreen.h"
#include "Src/Presentation/UI/Components/SoundSettingContentComponent.h"
#include "Src/Presentation/UI/Components/SoundSettingPanelComponent.h"

namespace nsApp
{
    namespace nsUI
    {
        void SoundSettingScreen::Build()
        {
            /* ルートに背景 → 中身の順で載せる。 */
            nsFramework::Entity& root = CreateRootEntity();
            pPanelComponent_ = &root.AddComponent<SoundSettingPanelComponent>();
            pContentComponent_ = &root.AddComponent<SoundSettingContentComponent>();
        }

        void SoundSettingScreen::Bind(nsPresentation::ISoundSettingData* pData)
        {
            /* データソースを保持し同期する。 */
            pHudData_ = pData;
            SyncFromDataSource();
        }

        void SoundSettingScreen::SetSelectedIndex(int index)
        {
            /* 選択行を保持し、見た目へ即反映する。 */
            selectedIndex_ = index;
            SyncFromDataSource();
        }

        void SoundSettingScreen::SyncFromDataSource()
        {
            if (pHudData_ == nullptr || pContentComponent_ == nullptr)
                return;

            /* Data + 選択行 → Content の見た目へ反映する。 */
            pContentComponent_->ApplyValues(pHudData_->GetMasterVolume(), pHudData_->GetBgmVolume(),
                                            pHudData_->GetSeVolume(), pHudData_->IsMasterMuted(),
                                            pHudData_->IsBgmMuted(), pHudData_->IsSeMuted(),
                                            pHudData_->IsControllerEnabled(), selectedIndex_);
        }

        void SoundSettingScreen::Update(float deltaTime)
        {
            if (!IsVisible())
                return;

            UIScreen::Update(deltaTime);
        }

        void SoundSettingScreen::Draw(RenderContext& rc)
        {
            if (!IsVisible())
                return;

            /* 描画直前に最新データを反映する。 */
            SyncFromDataSource();

            const Matrix& world = GetRootEntity().GetTransform().GetWorldMatrix();

            /* 背景 → 中身の順で描画する。 */
            if (pPanelComponent_ != nullptr)
                pPanelComponent_->OnDraw(rc, world);
            if (pContentComponent_ != nullptr)
                pContentComponent_->OnDraw(rc, world);
        }
    } // namespace nsUI
} // namespace nsApp
