// PatchNotesDimUI.h
#pragma once
#include "Src/UI/Framework/UIImage.h"

namespace nsApp
{
    namespace nsUI
    {
        /** @brief 背面の半透明暗転。 */
        class PatchNotesDimUI
        {
        public:
            /**
             * @brief 初期化する。
             */
            void Init();

            /**
             * @brief 更新する。
             */
            void Update();

            /**
             * @brief 描画する。
             * @param rc レンダリングコンテキスト
             */
            void Draw(RenderContext& rc);

            /**
             * @brief 表示状態を設定する。
             * @param visible 表示状態
             */
            void SetVisible(bool visible);


        private:
            UIImage dim_;
        };
    } // namespace nsUI
} // namespace nsApp

// PatchNotesDimUI.cpp
