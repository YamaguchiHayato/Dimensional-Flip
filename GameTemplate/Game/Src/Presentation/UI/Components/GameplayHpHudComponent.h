#pragma once
#include "Src/Presentation/UI/Components/IUIComponent.h"

/**
 * @file   GameplayHpHudComponent.h
 * @brief  HP 枠・ハート・数値を描画する Gameplay HUD Component。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class GameplayHpHudComponent
         * @brief HP 枠・ハート・数値を描画する Component。
         */
        class GameplayHpHudComponent : public IUIComponent
        {
        public:
            /**
             * @brief ハートの表示位置を設定する。
             * @param x ハート X。
             * @param y ハート Y。
             */
            void SetHeartPosition(float x, float y);

            /**
             * @brief 初回構築。
             */
            void OnBuild() override;

            /**
             * @brief 毎フレーム更新。Player から HP を取得する。
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
            /**
             * @brief Player GO から HP 表示を更新する。
             */
            void UpdateFromPlayer();

        private:
            float heartPosX_ = -900.0f; //! ハートの表示位置 X。
            float heartPosY_ = 500.0f;  //! ハートの表示位置 Y。

            SpriteRender flame_; //! HP 枠のスプライト。
            SpriteRender heart_; //! ハートのスプライト。
            FontRender hpFont_[5]; //! HP 数値のフォントレンダラー。
            Vector4 color_ = Vector4::White; //! HP 数値の色。
            wchar_t hpText_[64];             //! HP 数値のテキストバッファ。     
            bool spritesInitialized_ = false; //! < スプライトが初期化済みかどうか。
        };
    } // namespace nsUI
} // namespace nsApp
