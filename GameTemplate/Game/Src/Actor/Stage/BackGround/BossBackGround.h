#pragma once

#include "IBackGround.h"

namespace nsApp
{
    namespace nsStage
    {
        namespace nsBackGround
        {
            /**
             * @brief ボスステージ用背景。
             *
             * 3D モデル（BossBackGround.tkm）を 2D カメラ時のみ描画する。
             * 実描画は RenderToMainTarget() で Forward 直前に行う。
             */
            class BossBackGround : public IBackGround
            {
            public:
                BossBackGround() = default;
                virtual ~BossBackGround() = default;

            public:
                bool Start() override;
                void Update() override;

                /**
                 * @brief 描画処理（IGameObject 用。即時描画は行わない）。
                 */
                void Render(RenderContext& rc) override;

                /**
                 * @brief メインレンダーターゲットへ背景を描画する。
                 */
                void RenderToMainTarget(RenderContext& rc, RenderTarget& mainRT) override;

            private:
                ModelRender bossModel_; //!< ボス戦背景モデル。
            };

        } // namespace nsBackGround
    } // namespace nsStage
} // namespace nsApp
