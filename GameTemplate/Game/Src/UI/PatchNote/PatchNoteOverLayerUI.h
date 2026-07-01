#pragma once

#include "Src/UI/Framework/UIImage.h"

/**
 * @file   PatchNoteOverlayUI.h
 * @brief  パッチノート背面の暗転レイヤー。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class PatchNoteOverlayUI
         * @brief 画面全体を暗くする UIImage ラッパー。
         */
        class PatchNoteOverlayUI
        {
        public:
            /** @brief スプライトを初期化する。 */
            void Init();

            /** @brief Transform を Sprite に同期する。 */
            void Update();

            /** @brief 描画する。 */
            void Draw(RenderContext& rc);

            void SetVisible(bool visible);
            bool IsVisible() const;

            UITransform& GetTransform();
            UIImage& GetImage();

        private:
            UIImage image_; //! < 暗転レイヤーの UIImage。
        };
    } // namespace nsUI
} // namespace nsApp
