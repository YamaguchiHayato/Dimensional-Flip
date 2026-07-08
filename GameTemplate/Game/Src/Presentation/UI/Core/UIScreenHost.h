#pragma once

#include <memory>
#include "Src/Presentation/UI/Core/UIScreen.h"

/**
 * @file   UIScreenHost.h
 * @brief  UIScreen を IGameObject として動かす GO 境界。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class UIScreenHost
         * @brief 1画面 = 1GO。Update / Render から UIScreen を呼ぶ。
         * @note  派生クラスで CreateScreen() を実装する。
         */
        class UIScreenHost : public IGameObject
        {
        public:
            /**
             * @brief UIScreenHost を初期化する。
             * @return 生成した UIScreenHost。
             */
            bool Start() override;

            /**
             * @brief UIScreenHost を更新する。
             */
            void Update() override;

            /**
             * @brief UIScreenHost を描画する。
             * @param rc 描画コンテキスト。
             */
            void Render(RenderContext& rc) override;

            /**
             * @brief 保持している UIScreen を取得する。
             * @return UIScreen。未生成なら nullptr。
             */
            UIScreen* GetScreen() { return screen_.get(); }


        protected:
            /**
             * @brief 派生クラスで具体 UIScreen を生成する。
             * @return 生成した UIScreen。
             */
            virtual std::unique_ptr<UIScreen> CreateScreen() = 0;


        protected:
            std::unique_ptr<UIScreen> screen_;
        };
    } // namespace nsUI
} // namespace nsApp
