#pragma once

#include <functional>

#include "Src/Framework/IComponent.h"

namespace nsApp
{
    namespace nsFramework
    {
        /**
         * @class HealthComponent
         * @brief HP を保持し、変更時に通知するコンポーネント。
         */
        class HealthComponent : public IComponent
        {
        public:
            /**
             * @brief HP 変更時のコールバック型。
             * @param current 現在 HP。
             * @param max 最大 HP。
             */
            using HpChangedCallback = std::function<void(float current, float max)>;

            /* コンストラクタ。*/
            HealthComponent() = default;

            /**
             * @brief HP を初期化する。
             * @param maxHp 最大 HP。
             * @param currentHp 現在 HP。省略時は maxHp。
             */
            void Initialize(float maxHp, float currentHp = -1.0f);

            /**
             * @brief HP を初期化する。
             * @return 現在HP。
             */
            float GetCurrentHp() const { return currentHp_; }

            /**
             * @brief 現在 HP を取得する。
             * @return 最大 HP。
             */
            float GetMaxHp() const { return maxHp_; }

            /**
             * @brief 最大 HP をセットする。
             * @param hp 最大 HP。
             */
            void SetCurrentHp(float hp);

            /**
             * @brief 最大 HP を取得する。
             * @param amount ダメージ量。
             */
            void TakeDamage(float amount);

            /**
             * @brief HP 変更時コールバックを登録する。
             */
            void SetOnHpChanged(HpChangedCallback callback);


        private:
            /**
             * @brief HP 変更時コールバックを解除する。
             */
            void NotifyHpChanged();

        private:
            float maxHp_ = 0.0f;//! 最大HP。
            float currentHp_ = 0.0f;        //! HP。
            HpChangedCallback onHpChanged_; //! 時コールバック。
        };
    } // namespace nsFramework
} // namespace nsApp
