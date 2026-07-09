#include "stdafx.h"

#include "BossBriefingData.h"
#include "Src/Presentation/UI/Screens/BossBriefingScreen.h"

namespace nsApp
{
    namespace nsPresentation
    {
        void BossBriefingData::SetScreen(nsUI::BossBriefingScreen* pScreen)
        {
            pScreen_ = pScreen;
            SyncToScreen();
        }

        void BossBriefingData::SetSlidePaths(const std::vector<std::string>& paths)
        {
            slidePaths_ = paths;
            currentPageIndex_ = 0;
            SyncToScreen();
        }

        void BossBriefingData::ResetPage()
        {
            currentPageIndex_ = 0;
            SyncToScreen();
        }

        bool BossBriefingData::NextPage()
        {
            if (slidePaths_.empty())
                return false;

            if (currentPageIndex_ + 1 >= static_cast<int>(slidePaths_.size()))
                return false;

            ++currentPageIndex_;
            SyncToScreen();
            return true;
        }

        const char* BossBriefingData::GetSlidePath(int pageIndex) const
        {
            if (pageIndex < 0 || pageIndex >= static_cast<int>(slidePaths_.size()))
                return nullptr;

            return slidePaths_[pageIndex].c_str();
        }

        bool BossBriefingData::IsLastPage() const
        {
            if (slidePaths_.empty())
                return true;

            return currentPageIndex_ + 1 >= static_cast<int>(slidePaths_.size());
        }

        void BossBriefingData::SyncToScreen()
        {
            if (pScreen_ != nullptr)
                pScreen_->SyncFromDataSource();
        }
    } // namespace nsPresentation
} // namespace nsApp
