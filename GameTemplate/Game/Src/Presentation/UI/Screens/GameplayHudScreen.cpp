#include "stdafx.h"

#include "GameplayHudScreen.h"
#include "Src/Presentation/Data/IGameplayHubData.h"

namespace nsApp
{
    namespace nsUI
    {
        void GameplayHudScreen::SetDataSource(nsPresentation::IGameplayHudData* pDataSource)
        {
            pDataSource_ = pDataSource;
        }


        void GameplayHudScreen::SyncFromDataSource()
        {
            if (pDataSource_ == nullptr)
                return;
        }


        void GameplayHudScreen::Update() {}


        void GameplayHudScreen::Render(RenderContext& rc)
        {
            (void) rc;
        }
    } // namespace nsUI
} // namespace nsApp
