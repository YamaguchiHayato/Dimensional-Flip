#pragma once

#include "Src/Framework/IComponent.h"

namespace nsApp
{
    namespace nsPresentation
    {
        class IBossHudData;
    }

    namespace nsFramework
    {
        /**
         * @file   HealthComponent.h
         * @brief  HP 管理用 Framework Component。
         */

        /**
         * @class HealthComponent
         * @brief 現在 HP / 最大 HP を保持し、変更時に IBossHudData へ通知する。
         */
        class HealthComponent : public IComponent
        {
        public:
            /**
             * @brief 初期 HP を設定する。
             * @param maxHp        最大 HP。
             * @param pHudData     UI 通知先。nullptr なら通知しない。
             */
            void Initialize(float maxHp, nsPresentation::IBossHudData* pHudData);

            /**
             * @brief 現在 HP を設定する。
             * @param hp 現在 HP。
             */
            void SetCurrentHp(float hp);

            /**
             * @brief ダメージを与える。
             * @param damage ダメージ量。
             */
            void ApplyDamage(float damage);

            /**
             * @brief 現在のHPを取得する。
             * @return 現在のHP。
             */
            float GetCurrentHp() const { return currentHp_; }

            /**
             * @brief 最大HPを取得する。
             * @return 最大HP。
             */
            float GetMaxHp() const { return maxHp_; }


        private:
            /**
             * @brief IBossHudData へ現在 HP / 最大 HP を通知する。
             */
            void NotifyHud() const;


        private:
            float currentHp_ = 0.0f; //! 現在のHP。
            float maxHp_ = 0.0f;     //! 最大HP。
            nsPresentation::IBossHudData* pHudData_ = nullptr; //! UI 通知先。nullptr なら通知しない。
        };
    } // namespace nsFramework
} // namespace nsApp
