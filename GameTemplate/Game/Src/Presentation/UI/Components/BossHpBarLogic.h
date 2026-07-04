#pragma once

#include "Src/Framework/IComponent.h"

/**
 * @file   BossHpBarLogic.h
 * @brief  ボス HP バーの割合とダメージバー遅延を管理する Component。
 */

namespace nsApp
{
    namespace nsUI
    {
        class UISpriteView;

        /**
         * @class BossHpBarLogic
         * @brief current / damage の HP 割合を保持し、スプライトスケールに反映する。
         */
        class BossHpBarLogic : public nsFramework::IComponent
        {
        public:
            /**
             * @brief 描画対象スプライトを登録する。
             * @param pCurrentBar 現在 HP バー。
             * @param pDamageBar  ダメージバー。
             */
            void Setup(UISpriteView* pCurrentBar, UISpriteView* pDamageBar);

            /**
             * @brief HP 割合を設定する（0.0 ～ 1.0）。
             * @param percent HP 割合。
             */
            void SetHpPercent(float percent);

            /**
             * @brief IComponent::OnUpdate() をオーバーライドする。
             * @param deltaTime 前フレームからの経過時間（秒）。
             */
            void OnUpdate(float deltaTime) override;


        private:
            UISpriteView* pCurrentBar_ = nullptr;
            UISpriteView* pDamageBar_ = nullptr;
            float currentPercent_ = 1.0f;
            float damagePercent_ = 1.0f;
        };
    } // namespace nsUI
} // namespace nsApp
