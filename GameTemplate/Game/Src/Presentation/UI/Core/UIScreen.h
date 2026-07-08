#pragma once

#include <memory>

#include "Src/Framework/Entity.h"

/**
 * @file   UIScreen.h
 * @brief  UI 画面のルート。Build / Update / Draw の入口。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class UIScreen
         * @brief 1画面分の UI ツリーを保持する抽象クラス。
         */
        class UIScreen
        {
        public:
            virtual ~UIScreen() = default;

            /**
             * @brief 子 Entity / Component を生成する。一度だけ呼ぶ。
             */
            virtual void Build() = 0;

            /**
             * @brief 毎フレーム更新。
             * @param deltaTime 経過秒数。
             */
            virtual void Update(float deltaTime);

            /**
             * @brief 毎フレーム描画。
             * @param rc レンダリングコンテキスト。
             */
            virtual void Draw(RenderContext& rc);

            /**
             * @brief 表示 / 非表示を切り替える。
             * @param visible true で表示。
             */
            void SetVisible(bool visible);

            /**
             * @brief 表示中かどうか。
             * @return 表示中なら true。
             */
            bool IsVisible() const { return isVisible_; }

            /**
             * @brief ルート Entity を取得する。
             * @return ルート Entity。
             */
            nsFramework::Entity& GetRootEntity();
            const nsFramework::Entity& GetRootEntity() const;


        protected:
            /**
             * @brief ルート Entity を生成する。派生クラスの Build から呼ぶ。
             * @return 生成したルート Entity。
             */
            nsFramework::Entity& CreateRootEntity();


        private:
            std::unique_ptr<nsFramework::Entity> rootEntity_;
            bool isVisible_ = true;
        };
    } // namespace nsUI
} // namespace nsApp
