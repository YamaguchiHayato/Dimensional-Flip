#pragma once

#include "Src/Framework/IComponent.h"

/**
 * @file   UISpriteView.h
 * @brief  SpriteRender をラップする描画 Component。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class UISpriteView
         * @brief Entity のワールド行列位置にスプライトを描画する。
         */
        class UISpriteView : public nsFramework::IComponent
        {
        public:
            /**
             * @brief テクスチャとサイズを初期化する。
             * @param texturePath テクスチャパス。
             * @param width         幅。
             * @param height        高さ。
             */
            void Initialize(const char* texturePath, float width, float height);

            /**
             * @brief 描画スケールを設定する。
             * @param scale 描画スケール。
             */
            void SetScale(const Vector3& scale);

            /**
             * @brief ローカルオフセットを設定する。
             * @param offset ローカルオフセット。
             */
            void SetLocalOffset(const Vector3& offset);

            /**
             * @brief 描画する。
             * @param rc 描画コンテキスト。
             * @param ownerWorld オーナーのワールド行列。
             */
            void OnDraw(RenderContext& rc, const Matrix& ownerWorld) override;


        private:
            SpriteRender sprite_;
            Vector3 localOffset_ = Vector3::Zero;
            Vector3 drawScale_ = Vector3::One;
            bool isInitialized_ = false;
        };
    } // namespace nsUI
} // namespace nsApp
