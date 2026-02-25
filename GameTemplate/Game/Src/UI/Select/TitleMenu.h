#pragma once
#include "stdint.h"

// タイトルメニュー画面の種類。
enum class TitleMenuType : uint8_t
{
    GameStart, // ゲーム開始。
    Manual,    // マニュアル(遊び方)。
    GameEnd,   // ゲーム終了。
    Max
};


namespace app
{
    namespace nsUI
    {
        class TitleMenu : public IGameObject
        {
        public:
            TitleMenu() : curenntIndex_(0) {};
            virtual ~TitleMenu() = default;


        public:
            // 初期化処理。
            void Init();

            // メニュー画面の更新処理。
            // isUp: 上入力がされたか。
            // isDown: 下入力がされたか。
            // isDecide: 決定入力がされたか。
            bool Update(bool isUp, bool isDown, bool isDecide);


            // 描画処理。
            void Render(RenderContext& rc);


        // セッター。
        public:
            // 表示。
            inline void Open()
            {
                isActive_ = true;
            }

            // 非表示。
            inline void Close()
            {
                isActive_ = false;
            }

        // ゲッター。
        public:
            // 現在選択されている項目を取得する。
            inline TitleMenuType GetCurrentSelect() const
            {
                return static_cast<TitleMenuType>(curenntIndex_);
            }


        private:
            // 現在選択されている項目。
            uint8_t curenntIndex_ = 0;

            // メニューの項目の総数。(今回は3)
            FontRender menuFonts_[(int) TitleMenuType::Max];

            // 表示フラグ。
            bool isActive_ = false;
        };

    }
}
