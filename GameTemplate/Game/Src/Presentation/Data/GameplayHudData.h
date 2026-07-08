#pragma once
#include "Src/Presentation/Data/IGameplayHubData.h"

namespace nsApp
{
    namespace nsUI
    {
        class GameplayHudScreen;
    }

    namespace nsPresentation
    {
        /**
         * @class GameplayHudData
         * @brief インゲーム HUD 用データ。Game / Player から値を受け取る。
         */
        class GameplayHudData : public IGameplayHudData
        {
        public:
            /**
             * @brief コンストラクタ。
             * @note 定義は .cpp のみ（.h に {} を書くと C2084）。
             */
            GameplayHudData();

            /**
             * @brief 更新先 Screen を登録する。
             * @param pScreen GameplayHudScreen。nullptr 可。
             */
            void SetScreen(nsUI::GameplayHudScreen* pScreen);

            /**
             * @brief 残り秒数を設定する。
             */
            void SetTimerSeconds(int seconds);

            /**
             * @brief スコアを設定する。
             */
            void SetScore(int score);

            /**
             * @brief プレイヤー HP 比率を設定する (0.0〜1.0)。
             */
            void SetPlayerHpRatio(float ratio);

            /* IGameplayHudData */
            int GetTimerSeconds() const override;
            int GetScore() const override;
            float GetPlayerHpRatio() const override;

            /**
             * @brief 保持値を Screen へ反映する。
             */
            void SyncToScreen();

        private:
            nsUI::GameplayHudScreen* pScreen_;
            int timerSeconds_;
            int score_;
            float playerHpRatio_;
        };
    } /* namespace nsPresentation */
} /* namespace nsApp */
