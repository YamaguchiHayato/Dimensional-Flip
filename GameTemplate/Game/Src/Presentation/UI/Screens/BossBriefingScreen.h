#pragma once

#include "Src/Presentation/Data/IBossBriefingData.h"
#include "Src/Presentation/UI/Core/UIScreen.h"

namespace nsApp
{
    namespace nsUI
    {
        class BossBriefingSlideComponent;

        /**
         * @class BossBriefingScreen
         * @brief スライド画像 + ページ番号 + 操作ヒントを表示する。
         */
        class BossBriefingScreen : public UIScreen
        {
        public:
            void Build() override;
            void Update(float deltaTime) override;
            void Draw(RenderContext& rc) override;

            void Bind(nsPresentation::IBossBriefingData* pData);
            void SyncFromDataSource();

        private:
            nsPresentation::IBossBriefingData* pData_ = nullptr;
            BossBriefingSlideComponent* pSlideComponent_ = nullptr;

            FontRender pageLabel_;
            FontRender hintLabel_;
            bool isBuilt_ = false;
        };
    } // namespace nsUI
} // namespace nsApp
