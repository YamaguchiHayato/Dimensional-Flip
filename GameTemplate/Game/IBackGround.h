#pragma once

namespace nsApp
{
    namespace nsStage
    {
        namespace nsBackGround
        {
            /**
             * @brief ステージ背景の共通インターフェース。
             *
             * ScrollStageBackGround / BossBackGround などが実装する。
             */
            class IBackGround : public IGameObject
            {
            public:
                IBackGround() = default;
                virtual ~IBackGround() = default;

            public:
                /**
                 * @brief 初期化処理。
                 * @return 成功時 true。
                 */
                virtual bool Start() override = 0;

                /**
                 * @brief 更新処理。
                 */
                virtual void Update() override = 0;

                /**
                 * @brief 描画処理。
                 * @param rc レンダリングコンテキスト。
                 */
                virtual void Render(RenderContext& rc) override = 0;
            };

        } // namespace nsBackGround
    } // namespace nsStage
} // namespace nsApp
