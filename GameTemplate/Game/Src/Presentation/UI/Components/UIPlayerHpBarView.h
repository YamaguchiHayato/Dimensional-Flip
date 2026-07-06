#pragma once

#include "Src/Presentation/UI/Logic/PlayerHpBarLogic.h"

namespace nsApp
{
    namespace nsUI
    {
        class UIPlayerHpBarView
        {
        public:
            /**
             * @brief コンストラクタ。
             */
            UIPlayerHpBarView();

            /**
             * @brief 位置を設定する。
             * @param x X座標。
             * @param y Y座標。
             */
            void SetPosition(float x, float y);

            /**
             * @brief ロジックを適用する。
             * @param logic 適用するロジック。
             */
            void ApplyLogic(const PlayerHpBarLogic& logic);

            /**
             * @brief 描画する。
             * @param rc 描画コンテキスト。
             */
            void Draw(RenderContext& rc);


        private:
            float posX_; //! X座標。
            float posY_; //! Y座標。
            PlayerHpBarLogic cachedLogic_; //! キャッシュされたロジック。
        };
    } // namespace nsUI
} // namespace nsApp
