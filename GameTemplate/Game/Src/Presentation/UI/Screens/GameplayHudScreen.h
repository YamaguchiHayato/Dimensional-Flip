#pragma once

#include "Src/Presentation/UI/Core/UIScreen.h"
#include "Src/Presentation/UI/Logic/PlayerHpBarLogic.h"
#include "Src/Presentation/UI/Logic/ScoreDisplayLogic.h"
#include "Src/Presentation/UI/Logic/TImeDisplayLogic.h"
#include "Src/Presentation/UI/Widget/UICanvas.h"

/**
 * @file   GameplayHudScreen.h
 * @brief  Timer / Score / HP を UICanvas + Entity ツリーで描画する Screen。
 */

namespace nsApp
{
    namespace nsPresentation
    {
        class IGameplayHudData;
        class GameplayHudData;
    } // namespace nsPresentation

    namespace nsFramework
    {
        class Entity;
    }

    namespace nsUI
    {
        class GameplayTimerHudComponent;
        class GameplayScoreHudComponent;
        class GameplayHpHudComponent;

        /**
         * @class GameplayHudScreen
         * @brief Timer / Score / HP を UICanvas + Entity ツリーで描画する Screen。
         */
        class GameplayHudScreen : public UIScreen
        {
        public:
            /**
             * @brief コンストラクタ。
             */
            GameplayHudScreen() = default;

            /**
             * @brief HUD データソースを設定する。
             * @param pDataSource HUD データソース。
             */
            void SetDataSource(nsPresentation::IGameplayHudData* pDataSource);

            /**
             * @brief HUD データソースからロジックを同期する。
             */
            void SyncFromDataSource();

            /**
             * @brief Entity ツリーを構築する。
             */
            void Build() override;

            /**
             * @brief HUD を描画する。
             * @param rc 描画コンテキスト。
             */
            void Draw(RenderContext& rc) override;

            /**
             * @brief HUD データソースを接続する。
             * @param pData HUD データソース。
             */
            void ConnectToData(nsPresentation::GameplayHudData* pData);

            /**
             * @brief スコア部分だけ表示/非表示（チュートリアル用）。
             * @param visible true で表示。
             */
            void SetScoreVisible(bool visible);


        private:
            /**
             * @brief 描画可能かどうかを判定する。
             * @return 描画可能なら true。
             */
            bool CanDrawHud() const;

        private:
            nsPresentation::IGameplayHudData* pDataSource_ = nullptr; //! HUD データソース。

            UICanvas canvas_; //! < HUD のルート Canvas。
            nsFramework::Entity* pScorePanelEntity_ = nullptr; //! < スコアパネルの Entity。

            GameplayTimerHudComponent* pTimerComponent_ = nullptr; //! < タイマー表示の Component。
            GameplayScoreHudComponent* pScoreComponent_ = nullptr; //! < スコア表示の Component。
            GameplayHpHudComponent* pHpComponent_ = nullptr;       //! < HP 表示の Component。

            TimerDisplayLogic timerLogic_; //! < タイマー表示のロジック。
            ScoreDisplayLogic scoreLogic_; //! < スコア表示のロジック。
            PlayerHpBarLogic hpBarLogic_;  //! < HP バー表示のロジック。
        };
    } // namespace nsUI
} // namespace nsApp
