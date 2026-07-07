#pragma once

#include "Src/Presentation/UI/Components/UIPlayerHpBarView.h"
#include "Src/Presentation/UI/Components/UIScoreView.h"
#include "Src/Presentation/UI/Components/UITimerView.h"
#include "Src/Presentation/UI/Core/UIScreen.h"
#include "Src/Presentation/UI/Logic/PlayerHpBarLogic.h"
#include "Src/Presentation/UI/Logic/ScoreDisplayLogic.h"
#include "Src/Presentation/UI/Logic/TImeDisplayLogic.h"

namespace nsApp
{
    namespace nsPresentation
    {
        class IGameplayHudData;
        class GameplayHudData;
    }

    namespace nsUI
    {
        /**
         * @class GameplayHudScreen
         * @brief Timer / Score / HP をまとめて描画する Screen（新式 HUD）。
         *
         * UIScreen::Draw() は Entity ツリー用なので、
         * View 直描画は DrawHud() を Host から呼ぶ。
         */
        class GameplayHudScreen : public UIScreen
        {
        public:
            /**
             * @brief コンストラクタ。
             */
            GameplayHudScreen();

            /**
             * @brief GameplayHudScreen のコンストラクタ。
             * @param pDataSource HUD データソース。
             */
            void SetDataSource(nsPresentation::IGameplayHudData* pDataSource);

            /**
             * @brief HUD データソースを設定する。
             */
            void SyncFromDataSource();

            /**
             * @brief HUD データソースからロジックを同期する。
             */
            void Build() override;

            /**
             * @brief UIScreen::Build() の実装。ルート Entity だけ作る。
             * @param rc 描画コンテキスト。
             */
            void DrawHud(RenderContext& rc);

            /**
             * @brief HUD を描画する。Host::Render から呼ぶ。
             */
            void SetupLayout();

            /**
             * @brief HUD データソースを接続する。
             * @param pData HUD データソース。
             */
            void ConnectToData(nsPresentation::GameplayHudData* pData);

            /**
             * @brief スコア部分だけ表示/非表示（チュートリアル用）。
             */
            void SetScoreVisible(bool visible);


        private:
            nsPresentation::IGameplayHudData* pDataSource_ = nullptr; 

            TimerDisplayLogic timerLogic_;
            ScoreDisplayLogic scoreLogic_;
            PlayerHpBarLogic hpBarLogic_;

            UITimerView timerView_;
            UIScoreView scoreView_;
            UIPlayerHpBarView hpBarView_;
        };
    } // namespace nsUI
} // namespace nsApp
