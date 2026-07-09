#pragma once

#include "Src/Presentation/UI/Components/IUIComponent.h"

/**
 * @file   BossBriefingSlideComponent.h
 * @brief  ボス戦チュートリアルのスライド画像1枚を表示する。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class BossBriefingSlideComponent
         * @brief Data のパスからスライド画像を差し替えて描画する。
         */
        class BossBriefingSlideComponent : public IUIComponent
        {
        public:
            /**
             * @brief 表示するスライド画像を差し替える。
             * @param[in] path DDS パス。nullptr なら非表示。
             */
            void ApplySlidePath(const char* path);

            /**
             * @brief IUIComponent の OnBuild() をオーバーライドする。
             */
            void OnBuild() override;

            /**
             * @brief IUIComponent の OnUpdate() をオーバーライドする。
             * @param deltaTime 前フレームからの経過時間（秒）。
             */
            void OnUpdate(float deltaTime) override;

            /**
             * @brief IUIComponent の OnDraw() をオーバーライドする。
             * @param rc レンダリングコンテキスト
             * @param ownerWorld 親のワールド行列
             */
            void OnDraw(RenderContext& rc, const Matrix& ownerWorld) override;


        private:
            SpriteRender slideSprite_; //!< スライド画像。
            bool isBuilt_ = false;     //!< Build 済みか。

            std::string currentPath_; //!< 現在読み込み中のパス。

            Vector3 slidePos_{0.0f, 0.0f, 0.0f}; //! < スライド画像の位置。
            float slideW_ = 1280.0f;             //! < スライド画像の幅。
            float slideH_ = 720.0f;              //! < スライド画像の高さ。
        };
    } // namespace nsUI
} // namespace nsApp
