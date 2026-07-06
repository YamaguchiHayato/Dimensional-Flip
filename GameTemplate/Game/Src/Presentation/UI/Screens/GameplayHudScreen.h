#pragma once

#include "Src/Presentation/UI/Core/UIScreen.h"

namespace nsApp
{
    namespace nsPresentation
    {
        class IGameplayHudData;
    }

    namespace nsUI
    {
        /**
         * @class GameplayHudScreen
         * @brief Timer / Score / HP をまとめて描画する Screen。
         */
        class GameplayHudScreen : public UIScreen
        {
        public:
            /**
             * @brief データソースを設定する。
             * @param pDataSource IGameplayHudData のポインタ。
             */
            void SetDataSource(nsPresentation::IGameplayHudData* pDataSource);

            /**
             * @brief データソースから値を同期する。
             */
            void SyncFromDataSource() ;

            /**
             * @brief タイマーの秒数を取得する。
             */
            void Update();

            /**
             * @brief 描画する。
             * @param rc RenderContext の参照。
             */
            void Render(RenderContext& rc);


        private:
            nsPresentation::IGameplayHudData* pDataSource_ = nullptr; //! GameplayHudScreen のデータソース。
        };
    } // namespace nsUI
} // namespace nsApp
