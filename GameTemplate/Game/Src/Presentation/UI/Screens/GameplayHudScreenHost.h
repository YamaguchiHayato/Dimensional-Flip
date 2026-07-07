#pragma once

#include <memory>

#include "Src/Presentation/UI/Core/UIScreenHost.h"

namespace nsApp
{
    namespace nsUI
    {
        class GameplayHudScreen;

        /**
         * @class GameplayHudScreenHost
         * @brief GameplayHudScreen を GO として保持する Host。
         */
        class GameplayHudScreenHost : public UIScreenHost
        {
        public:
            /**
             * @brief GameplayHudScreenHost のコンストラクタ。
             */
            bool Start() override;

            /**
             * @brief GameplayHudScreenHost のコンストラクタ。
             * @param rc 描画コンテキスト。
             */
            void Render(RenderContext& rc) override; 

            /**
             * @brief GameplayHudScreenHost の描画。UIScreenHost::Render をオーバーライドして GameplayHudScreen::DrawHudを呼ぶ。
             * @return 描画成功なら true。
             */
            GameplayHudScreen* GetGameplayHudScreen();


        protected:
            /**
             * @brief GameplayHudScreen を生成する。UIScreenHost::CreateScreen をオーバーライド。
             * @return GameplayHudScreen の unique_ptr。
             */
            std::unique_ptr<UIScreen> CreateScreen() override;
        };
    } // namespace nsUI
} // namespace nsApp
