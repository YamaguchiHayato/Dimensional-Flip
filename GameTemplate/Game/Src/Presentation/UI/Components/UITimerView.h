#pragma once

namespace nsApp
{
    namespace nsUI
    {
        class TimerDisplayLogic;

        /**
         * @class UITimerView
         * @brief タイマー残り秒数を Font で描画する View。
         *
         * 旧 NumberUI と同じく FontRender + "%02d" 表示。
         * GameplayHubUiNames の桁スプライトは使わない。
         */
        class UITimerView
        {
        public:
            /**
             * @brief UITimerView のコンストラクタ。
             */
            UITimerView();

            /**
             * @brief UITimerView のデストラクタ。
             * @param x X 座標。
             * @param y Y 座標。
             */
            void SetPosition(float x, float y);

            /**
             * @brief UITimerView のデストラクタ。
             * @param spacing 桁間隔。
             */
            void SetDigitSpacing(float spacing);

            /**
             * @brief UITimerView のデストラクタ。
             * @param logic タイマー表示ロジック。
             */
            void ApplyLogic(const TimerDisplayLogic& logic);

            /**
             * @brief UITimerView の描画。
             * @param rc 描画コンテキスト。
             */
            void Draw(RenderContext& rc);


        private:
            /**
             * @brief UITimerView のフォントを初期化する。
             */
            void InitFont();

            /**
             * @brief 旧 TimerUI の背景バーを初期化する。
             */
            void InitTimerBar();


        private:
            float posX_; //! < X 座標。
            float posY_;         //! < Y 座標。
            float digitSpacing_; // 未使用（将来スプライト化したとき用）
            bool timerBarInitialized_;


            SpriteRender timerBar_; 
            FontRender timerFont_; //! < タイマー用フォントレンダラー。
            wchar_t timerText_[8]; //! < タイマー表示用テキストバッファ。
            int cachedSeconds_;    //! < キャッシュされた秒数。前回描画時の秒数を保持しておく。
        };
    } /* namespace nsUI */
} /* namespace nsApp */
