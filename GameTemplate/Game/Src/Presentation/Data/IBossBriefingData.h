#pragma once

/**
 * @file   IBossBriefingData.h
 * @brief  ボス戦チュートリアル（スライド説明）のデータ読み取り口。
 */

namespace nsApp
{
    namespace nsPresentation
    {
        /**
         * @class IBossBriefingData
         * @brief 現在ページ・総ページ数・スライド画像パスを提供する。
         */
        class IBossBriefingData
        {
        public:
            /* コンストラクタ。*/
            virtual ~IBossBriefingData() = default;

            /**
             * @brief 現在ページ（0 始まり）を返す。
             */
            virtual int GetCurrentPageIndex() const = 0;

            /**
             * @brief 総ページ数を返す。
             */
            virtual int GetPageCount() const = 0;

            /**
             * @brief 指定ページのスライド画像パスを返す。
             * @param[in] pageIndex 0 始まりのページ番号。
             */
            virtual const char* GetSlidePath(int pageIndex) const = 0;
        };
    } // namespace nsPresentation
} // namespace nsApp
