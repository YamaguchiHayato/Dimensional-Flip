#pragma once

#include "Src/Presentation/UI/Components/IUIComponent.h"

/**
 * @file   BossStatusHudComponent.h
 * @brief  ボスアイコン + HP バー（枠/current/damage）を描画する Component。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class BossStatusHudComponent
         * @brief ボスステータス HUD（HPバー）表示専用 Component。
         */
        class BossStatusHudComponent : public IUIComponent
        {
        public:
            /**
             * @brief HP 割合を設定する（0.0 ～ 1.0）。
             * @param percent HP 割合。
             */
            void SetHpPercent(float percent);

            /**
             * @brief 初回構築。
             */
            void OnBuild() override;

            /**
             * @brief 毎フレーム更新（ダメージバー遅延）。
             * @param deltaTime 経過秒数。
             */
            void OnUpdate(float deltaTime) override;

            /**
             * @brief 描画。
             * @param rc 描画コンテキスト。
             * @param ownerWorld 親 Entity のワールド行列。
             */
            void OnDraw(RenderContext& rc, const Matrix& ownerWorld) override;


        private:
            SpriteRender bossIcon_; //! ボスアイコン。
            SpriteRender hpFrame_;  //! HPバーの枠。
            SpriteRender hpCurrent_; //! HPバーの現在値。
            SpriteRender hpDamage_;  //! HPバーのダメージ値。

            float currentPercent_ = 1.0f; //! < 現在の HP 割合。
            float damagePercent_ = 1.0f;  //! < ダメージバーの HP 割合。
            bool isBuilt_ = false;        //! < 初回構築済みかどうか。
        };
    } // namespace nsUI
} // namespace nsApp
