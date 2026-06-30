#pragma once

#include <cstdint>

/**
 * @file   TitleMenu.h
 * @brief  タイトル画面のメニュー UI。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @enum TitleMenuType
         * @brief タイトルメニューの選択項目。
         */
        enum class TitleMenuType : uint8_t
        {
            GameStart, //!< ゲーム開始
            Manual,    //!< 操作説明
            GameEnd,   //!< 終了
            Max
        };

        /**
         * @class TitleMenu
         * @brief 上下入力で項目を選び、決定で結果を返すメニュー GO。
         */
        class TitleMenu : public IGameObject
        {
        public:
            /* コンストラクタとデストラクタ。*/
            TitleMenu() : curenntIndex_(0) {}
            virtual ~TitleMenu() = default;

            /**
             * @brief メニュー項目のフォントを初期化する。
             */
            void Init();

            /**
             * @brief メニュー入力を処理する。
             * @param isUp     上入力があったか。
             * @param isDown   下入力があったか。
             * @param isDecide 決定入力があったか。
             * @return 決定された場合 true。
             */
            bool Update(bool isUp, bool isDown, bool isDecide);

            /**
             * @brief メニュー項目を描画する。
             * @param rc レンダリングコンテキスト。
             */
            void Render(RenderContext& rc);

            /**
             * @brief メニューを表示する。
             */
            inline void Open()  { isActive_ = true; }

            /**
             * @brief メニューを非表示にする。
             */
            inline void Close() { isActive_ = false; }

            /**
             * @brief 現在選択中の項目を返す。
             * @return 選択中の TitleMenuType。
             */
            inline TitleMenuType GetCurrentSelect() const
            {
                return static_cast<TitleMenuType>(curenntIndex_);
            }

        private:
            uint8_t curenntIndex_ = 0;                        //!< 選択中インデックス
            FontRender menuFonts_[(int)TitleMenuType::Max];   //!< 各項目のフォント
            bool isActive_ = false;                           //!< 表示中かどうか
        };
    } // namespace nsUI
} // namespace nsApp

using TitleMenuType = nsApp::nsUI::TitleMenuType;
using TitleMenu     = nsApp::nsUI::TitleMenu;
