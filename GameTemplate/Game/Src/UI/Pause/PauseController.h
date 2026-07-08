#pragma once
#include "stdint.h"


namespace nsApp
{
    namespace nsUI
    {
        class PauseMenuUI;
    }

    namespace nsCore
    {
        class Game;

        /**
         * @enum-class PauseController 
         * @brief ポーズ画面のアクションを表す列挙型。
         * @details この列挙型は、ポーズ画面の状態遷移を管理するために使用されます。
         */
        enum class PauseAction : uint8_t
        {
            None, //! < アクションなし。
            Resume, // ! < ポーズ解除。
            BackToTitle, //! < タイトルに戻る。
        };

        /**
         * @file PauseController.h
         * @brief ポーズ入力と開閉状態を管理するコントローラ。
         */
        class PauseController
        {
        public:
            /**
             * @brief 初期化。
             * @param pPauseMenu ポーズメニューUIのポインタ。
             */
            inline void Initialize(nsUI::PauseMenuUI* pPauseMenu)
            {
                pPauseMenu_ = pPauseMenu;
            }

            /**
             * @brief ポーズ状態を更新し、必要に応じてアクションを返す。
             * @param pGame ゲームのポインタ。
             * @return 
             */
            PauseAction Update(Game* pGame);

            /**
             * @brief ポーズ状態を取得する。
             * @return true: ポーズ中, false: ポーズ解除
             */
            bool IsPaused() const { return isPaused_; }

            /**
             * @brief ポーズ画面を開く。
             */
            void Open();

            /**
             * @brief ポーズ画面を閉じる。
             */
            void Close();


        private:
            /**
             * @brief ポーズ画面を開くことができるかどうかを判定する。
             * @param pGame ゲームのポインタ。
             */
            bool CanOpen(Game* pGame) const;

            /**
             * @brief ポーズ画面を閉じることができるかどうかを判定する。
             */
            bool IsPauseTogglePressed() const;


        private:
            nsUI::PauseMenuUI* pPauseMenu_ = nullptr; //! < ポーズメニューUIのポインタ。
            bool isPaused_ = false;                   //! < ポーズ状態のフラグ。
        };
    } // namespace nsCore
} // namespace nsApp
