#include "stdafx.h"

#include "BossBriefingScreen.h"
#include "Src/Presentation/Data/BossBriefingData.h"
#include "Src/Presentation/UI/Components/BossBriefingSlideComponent.h"

namespace nsApp
{
    namespace nsUI
    {
        void BossBriefingScreen::Build()
        {
            if (isBuilt_)
                return;

            nsFramework::Entity& root = CreateRootEntity();
            pSlideComponent_ = &root.AddComponent<BossBriefingSlideComponent>();

            pageLabel_.SetScale(1.0f);
            pageLabel_.SetColor({1.0f, 1.0f, 0.2f, 1.0f});
            pageLabel_.SetPosition({-580.0f, 320.0f, 0.0f});

            hintLabel_.SetScale(1.1f);
            hintLabel_.SetColor({1.0f, 1.0f, 1.0f, 1.0f});
            hintLabel_.SetPosition({-200.0f, -320.0f, 0.0f});

            isBuilt_ = true;
        }

        void BossBriefingScreen::Bind(nsPresentation::IBossBriefingData* pData)
        {
            pData_ = pData;
            SyncFromDataSource();
        }

        void BossBriefingScreen::SyncFromDataSource()
        {
            if (pData_ == nullptr)
                return;

            /* 現在ページの画像を差し替える。 */
            if (pSlideComponent_ != nullptr)
                pSlideComponent_->ApplySlidePath(pData_->GetSlidePath(pData_->GetCurrentPageIndex()));

            wchar_t buf[128] = {};
            const int page = pData_->GetCurrentPageIndex() + 1;
            const int total = pData_->GetPageCount();
            swprintf_s(buf, L"%d / %d", page, total > 0 ? total : 1);
            pageLabel_.SetText(buf);

            /* 最終ページだけ「ボス戦開始」と表示する。 */
            bool isLast = false;
            if (auto* pConcrete = dynamic_cast<nsPresentation::BossBriefingData*>(pData_))
                isLast = pConcrete->IsLastPage();
        }

        void BossBriefingScreen::Update(float deltaTime)
        {
            if (!IsVisible())
                return;

            UIScreen::Update(deltaTime);
        }

        void BossBriefingScreen::Draw(RenderContext& rc)
        {
            if (!IsVisible())
                return;

            SyncFromDataSource();

            const Matrix& world = GetRootEntity().GetTransform().GetWorldMatrix();

            if (pSlideComponent_ != nullptr)
                pSlideComponent_->OnDraw(rc, world);

            pageLabel_.Draw(rc);
        }
    } // namespace nsUI
} // namespace nsApp
