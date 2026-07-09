#include "stdafx.h"

#include "BossBriefingController.h"
#include "Src/Presentation/Data/BossBriefingData.h"
#include "Src/Presentation/UI/Screens/BossBriefingScreen.h"
#include "Src/Presentation/UI/Screens/BossBriefingScreenHost.h"

namespace nsApp
{
    namespace nsCore
    {
        void BossBriefingController::Initialize(nsPresentation::BossBriefingData* pData)
        {
            pData_ = pData;
            pScreen_ = nullptr;
            pHost_ = nullptr;
            isOpen_ = false;
            isFinished_ = false;
        }


        void BossBriefingController::EnsureHost()
        {
            if (pHost_ != nullptr)
                return;

            pHost_ = FindGO<nsUI::BossBriefingScreenHost>("BossBriefingScreenHost");
            if (pHost_ == nullptr)
            {
                pHost_ = NewGO<nsUI::BossBriefingScreenHost>(2, "BossBriefingScreenHost");
                if (pHost_ != nullptr)
                    pHost_->StartWrapper();
            }

            if (pHost_ != nullptr)
                pScreen_ = pHost_->GetBossBriefingScreen();

            /* Data と Screen を接続する。 */
            if (pData_ != nullptr && pScreen_ != nullptr)
            {
                pData_->SetScreen(pScreen_);
                pScreen_->Bind(pData_);
            }
        }


        void BossBriefingController::Open()
        {
            if (isOpen_)
                return;

            EnsureHost();

            isOpen_ = true;
            isFinished_ = false;

            if (pData_ != nullptr)
                pData_->ResetPage();

            if (pScreen_ != nullptr)
                pScreen_->SetVisible(true);

            /* 説明中はゲーム時間を止める（カットインと同じ）。 */
            g_gameTime->EnableFixedFrameDeltaTime(0.0f);
        }


        void BossBriefingController::Close()
        {
            if (!isOpen_)
                return;

            isOpen_ = false;

            if (pScreen_ != nullptr)
                pScreen_->SetVisible(false);

            g_gameTime->DisableFixedFrameDeltaTime();
        }


        void BossBriefingController::Update()
        {
            if (!isOpen_ || pData_ == nullptr)
                return;

            if (consumedInputThisFrame_)
                return;

            const bool next = g_pad[0]->IsTrigger(enButtonA);
            const bool skip = g_pad[0]->IsTrigger(enButtonStart) || g_pad[0]->IsTrigger(enButtonSelect);

            if (skip)
            {
                consumedInputThisFrame_ = true;
                isFinished_ = true;
                Close();
                return;
            }

            if (!next)
                return;

            consumedInputThisFrame_ = true;

            if (pData_->IsLastPage())
            {
                isFinished_ = true;
                Close();
                return;
            }

            pData_->NextPage();
        }
    } // namespace nsCore
} // namespace nsApp
