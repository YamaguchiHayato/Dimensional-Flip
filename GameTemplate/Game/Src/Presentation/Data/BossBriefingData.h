#pragma once

#include <string>
#include <vector>

#include "IBossBriefingData.h"

/**
 * @file   BossBriefingData.h
 * @brief  ボス戦チュートリアルのページデータ保持。
 */

namespace nsApp
{
    namespace nsUI
    {
        class BossBriefingScreen;
    }

    namespace nsPresentation
    {
        /**
         * @class BossBriefingData
         * @brief スライド枚数・現在ページを保持し Screen へ同期する。
         */
        class BossBriefingData : public IBossBriefingData
        {
        public:
            /**
             * @brief 反映先 Screen を登録する。
             */
            void SetScreen(nsUI::BossBriefingScreen* pScreen);

            /**
             * @brief スライド画像パス一覧を設定する（第4段で本番データを入れる）。
             */
            void SetSlidePaths(const std::vector<std::string>& paths);

            /**
             * @brief 先頭ページへ戻す。
             */
            void ResetPage();

            /**
             * @brief 次ページへ進める。
             * @return 進めたら true。最終ページなら false。
             */
            bool NextPage();

            /**
             * @brief 次ページへ進める。
             * @return 進めたら true。最終ページなら false。
             */
            int GetCurrentPageIndex() const override
            {
                return currentPageIndex_;
            }

            /**
             * @brief 現在ページ（0 始まり）を返す。
             * @return 現在ページのインデックス（0 始まり）
             */
            int GetPageCount() const override
            {
                return static_cast<int>(slidePaths_.size());
            }


            const char* GetSlidePath(int pageIndex) const override;

            /**
             * @brief 最終ページか。
             */
            bool IsLastPage() const;

            /**
             * @brief Screen へ現在値を反映する。
             */
            void SyncToScreen();

        private:
            nsUI::BossBriefingScreen* pScreen_ = nullptr; //!< 反映先。
            std::vector<std::string> slidePaths_;         //!< スライド画像パス一覧。
            int currentPageIndex_ = 0;                    //!< 現在ページ。
        };
    } // namespace nsPresentation
} // namespace nsApp
