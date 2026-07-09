#pragma once

#include <stdint.h>

/**
 * @file   BossBriefingController.h
 * @brief  ボス戦チュートリアル（スライド）の開閉・ページ送り。
 */

namespace nsApp
{
    namespace nsUI
    {
        class BossBriefingScreen;
        class BossBriefingScreenHost;
    } // namespace nsUI

    namespace nsPresentation
    {
        class BossBriefingData;
    }

    namespace nsCore
    {
        /**
         * @class BossBriefingController
         * @brief スライド説明の入力と終了判定（非 GO）。
         */
        class BossBriefingController
        {
        public:
            /**
             * @brief Data を登録する。Screen は Open 時に Host から取得する。
             */
            void Initialize(nsPresentation::BossBriefingData* pData);

            /**
             * @brief スライド説明を開く。Open 後は Update を毎フレーム呼ぶ。
             */
            void Open();

            /**
             * @brief スライド説明を閉じる。Close 後は Update を呼ばない。
             */
            void Close();

            /**
             * @brief スライド説明が開いているかどうかを取得する。
             * @return true: 開いている, false: 閉じている
             */
            bool IsOpen() const { return isOpen_; }

            /**
             * @brief スライド説明が終了したかどうかを取得する。
             * @return true: 終了した, false: 終了していない
             */
            bool IsFinished() const { return isFinished_; }

            /**
             * @brief 毎フレーム呼ぶ。A で次ページ、Start でスキップ。
             */
            void Update();

            /**
             * @brief Update 内での入力消費フラグをリセットする。Update の最初に呼ぶこと。
             */
            void BeginInputFrame() { consumedInputThisFrame_ = false; }


        private:
            /**
             * @brief Host を確保する。Screen は Host から取得する。
             */
            void EnsureHost();


        private:
            nsPresentation::BossBriefingData* pData_ = nullptr;
            nsUI::BossBriefingScreen* pScreen_ = nullptr;
            nsUI::BossBriefingScreenHost* pHost_ = nullptr;

            bool isOpen_ = false;
            bool isFinished_ = false;
            bool consumedInputThisFrame_ = false; //!< 同一フレーム二重 Update 対策。
        };
    } // namespace nsCore
} // namespace nsApp
