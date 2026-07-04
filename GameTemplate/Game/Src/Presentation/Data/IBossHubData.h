#pragma once

/**
 * @file   IBossHudData.h
 * @brief  ボス HUD 向け HP データの受け渡しインターフェース。
 * @note   UI は Boss を Find しない。この interface 経由で値だけ受け取る。
 */

namespace nsApp
{
    namespace nsPresentation
    {
        /**
         * @class IBossHudData
         * @brief ボス HP 表示用データソース。
         */
        class IBossHudData
        {
        public:
            virtual ~IBossHudData() = default;

            /**
             * @brief 現在 HP と最大 HP を UI 側へ通知する。
             * @param currentHp 現在 HP。
             * @param maxHp     最大 HP。
             */
            virtual void SetHp(float currentHp, float maxHp) = 0;

            /**
             * @brief 現在 HP を取得する。
             * @return 現在 HP。
             */
            virtual float GetCurrentHp() const = 0;

            /**
             * @brief 最大 HP を取得する。
             * @return 最大 HP。
             */
            virtual float GetMaxHp() const = 0;
        };
    } // namespace nsPresentation
} // namespace nsApp
