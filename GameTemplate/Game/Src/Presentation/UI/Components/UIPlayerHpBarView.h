#pragma once

#include "Src/Presentation/UI/Logic/PlayerHpBarLogic.h"

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class UIPlayerHpBarView
         * @brief プレイヤー HP をスプライト + Font で描画（旧 HPbarUI 互換）。
         */
        class UIPlayerHpBarView
        {
        public:
            /**
             * @brief コンストラクタ。
             */
            UIPlayerHpBarView();

            /**
             * @brief 座標をセットする。
             * @param x X座標。
             * @param y Y座標。
             */
            void SetPosition(float x, float y);

            /**
             * @brief プレイヤー HP ロジックを適応する。
             * @param logic プレイヤー HP ロジック。
             */
            void ApplyLogic(const PlayerHpBarLogic& logic);

            /**
             * @brief 描画する。
             * @param rc 描画コンテキスト。
             */
            void Draw(RenderContext& rc);


        private:
            /**
             * @brief スプライトを初期化する。
             */
            void InitSprites();

            /**
             * @brief フォントを初期化する。
             */
            void InitHpFonts();

            /**
             * @brief プレイヤーからの情報を元に描画内容を更新する。
             */
            void UpdateFromPlayer();


        private:
            float heartPosX_; //! < ハートの X 座標。
            float heartPosY_; //! < ハートの Y 座標。

            PlayerHpBarLogic cachedLogic_; //! < 適応済みのプレイヤー HP ロジック。

            SpriteRender flame_; // 枠（progressBar/flame）
            SpriteRender heart_; // ハート（progressBar/Heart）

            FontRender hpFont_[5]; // 0-3: 縁取り、4: 本体
            wchar_t hpText_[64];
            bool spritesInitialized_;
        };
    } // namespace nsUI
} // namespace nsApp
