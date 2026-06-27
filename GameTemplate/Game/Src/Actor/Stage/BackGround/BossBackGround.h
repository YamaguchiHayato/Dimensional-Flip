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
             */
            class BossBackGround : public IBackGround
            {
            public:
                BossBackGround() = default;
                virtual ~BossBackGround() = default;

            public:
                /**
                 * @brief 初期化処理。
                 * @return 成功時 true。
                 */
                bool Start() override;

                /**
                 * @brief 更新処理。
                 */
                void Update() override;

                /**
                 * @brief 描画処理。
                 * @param rc レンダリングコンテキスト。
                 */
                void Render(RenderContext& rc) override;

            private:
                ModelRender bossModel_; //!< ボス戦背景モデル。
            };

        } // namespace nsBackGround
    } // namespace nsStage
} // namespace nsApp
