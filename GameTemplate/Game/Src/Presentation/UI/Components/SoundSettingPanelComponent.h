#pragma once

#include "Src/Presentation/UI/Components/IUIComponent.h"

/**
 * @file   SoundSettingPanelComponent.h
 * @brief  Setting 背景パネル（base.DDS）を描画する Component。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class SoundSettingPanelComponent
         * @brief モックの Main UI 背景を描画する。
         */
        class SoundSettingPanelComponent : public IUIComponent
        {
        public:
            /**
             * @brief スプライトを初期化する。
             */
            void OnBuild() override;

            /**
             * @brief 更新処理。
             * @param deltaTime 経過秒数。
             */
            void OnUpdate(float deltaTime) override;

            /**
             * @brief 描画処理。
             * @param rc レンダリングコンテキスト。
             * @param ownerWorld オーナーのワールド行列。
             */
            void OnDraw(RenderContext& rc, const Matrix& ownerWorld) override;


        private:
            SpriteRender baseSprite_; //! base.DDS。
            bool isBuilt_ = false;    //! < ビルド済みフラグ。

            /* レイアウト（必要に応じて後で調整） */
            Vector3 basePos_{0.0f, 0.0f, 0.0f};
            float baseW_ = 1280.0f;
            float baseH_ = 720.0f;

            const char* pathBase_ = "Assets/UI/setting/base.DDS";
        };
    } // namespace nsUI
} // namespace nsApp
