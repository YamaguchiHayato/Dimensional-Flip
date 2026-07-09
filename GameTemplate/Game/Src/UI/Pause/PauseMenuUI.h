#pragma once

#include "Src/UI/UIBase.h"

/**
 * @file PauseMenuUI.h
 * @brief ポーズ画面の見た目専用UI。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class PauseMenuUI 
         * @brief ポーズ画面の見た目専用UI。
         */
        class PauseMenuUI : public UIBase
        {
        public:
            /* コンストラクタとデストラクタ。*/
            PauseMenuUI() = default;
            virtual ~PauseMenuUI() = default;


        public:
            /**
             * @brief UIBase の Start() をオーバーライドする。
             * @return true: 成功, false: 失敗
             */
            bool Start() override;

            /**
             * @brief UIBase の Update() をオーバーライドする。
             */
            void Update() override;

            /**
             * @brief UIBase の Render() をオーバーライドする。
             * @param rc レンダリングコンテキスト
             */
            void Render(RenderContext& rc) override;

            /**
             * @brief ポーズ画面を開く。
             */
            void Open() { isOpen_ = true; }

            /**
             * @brief ポーズ画面を閉じる。
             */
            void Close() { isOpen_ = false; }

            /**
             * @brief ポーズ画面が開いているかどうかを取得する。
             * @return true: 開いている, false: 閉じている
             */
            bool IsOpen() const { return isOpen_; }

            /**
             * @brief 選択中の項目インデックスを取得する。
             * @param index 選択中の項目インデックス。
             */
            void SetSelectedIndex(int index);


        private:
            /**
             * @brief 選択中の項目の見た目を更新する。
             */
            void ApplySelectionVisual();


        private:
            SpriteRender pauseSprite_;   //!< pose.DDS
            FontRender restartFont_;     //!< "Restart" の文字列
            FontRender returnWorld_;     //!< "Return to World" の文字列
            FontRender returnTitle_;     //!< "Return to Title" の文字列
            bool isOpen_ = false;        //!< 開閉状態。
            int selectedIndex_ = 0;      //! < 選択中の項目インデックス。

        };
    }
}

using PauseMenuUI = nsApp::nsUI::PauseMenuUI;
