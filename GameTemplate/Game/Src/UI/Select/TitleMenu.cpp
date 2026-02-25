#include "stdafx.h"
#include "Src/UI/Select/TitleMenu.h"
#include "Src/Core/SoundManager.h"

namespace app
{
    namespace nsUI
    {
       void TitleMenu::Init()
        {
            // メニュー画面の項目のテキストを設定。
            menuFonts_[(int) TitleMenuType::GameStart].SetText(L"Game Start");
            menuFonts_[(int) TitleMenuType::Manual].SetText(L"Manual");
            menuFonts_[(int) TitleMenuType::GameEnd].SetText(L"Game End");

            // ゲームスタートコマンドテキストの座標。
            menuFonts_[(int) TitleMenuType::GameStart].SetPosition({-150.0f, -100.0f, 0.0f});

            // マニュアルコマンドテキストの座標。
            menuFonts_[(int) TitleMenuType::Manual].SetPosition({-150.0f,  -200.0f, 0.0f});

            // ゲーム終了コマンドテキストの位置
            menuFonts_[(int) TitleMenuType::GameEnd].SetPosition({-150.0f, -300.0f, 0.0f});

            // --- 3. 共通設定（サイズや影） ---
            for (uint8_t i = 0; i < (int) TitleMenuType::Max; i++)
            {
                // 基本の大きさ（Updateで上書きされるので、ここでの設定は初期値）
                menuFonts_[i].SetScale(2.5f);

                // 影の設定
                menuFonts_[i].SetShadowParam(true, 2.0f, {0.0f, 0.0f, 0.0f, 1.0f});

                // 中央揃えの基準点
                menuFonts_[i].SetPivot(0.5f, 0.5f);
            }
        }


       bool TitleMenu::Update(bool isUp, bool isDown, bool isDecide)
       {
            // --- カーソル移動処理 ---
            // 上移動。 
            if (isUp)
            {
                curenntIndex_--;
                if (curenntIndex_ < 0)
                    curenntIndex_ = (int) TitleMenuType::Max - 1;

                // 効果音の再生。
                app::core::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_CursorMove, 2.0f);
            }
            // 下移動。
            if (isDown)
            {
                curenntIndex_++;
                if (curenntIndex_ >= (int) TitleMenuType::Max)
                    curenntIndex_ = 0;

                // 効果音の再生。
                app::core::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_CursorMove, 2.0f);
            }


            // --- 色の更新処理 ---
            // 選ばれているものは「赤」、そうでないものは「白」にする
            for (int i = 0; i < (int) TitleMenuType::Max; ++i)
            {
                if (i == curenntIndex_)
                {
                    // 選択中：赤色 (RGBA)
                    menuFonts_[i].SetColor(1.0f, 0.0f, 0.0f, 1.0f);
                    // 選択時に少し大きくする。
                    menuFonts_[i].SetScale(1.2f);
                }
                else
                {
                    // 非選択：白色
                    menuFonts_[i].SetColor(1.0f, 1.0f, 1.0f, 1.0f);
                    // 通常の大きさ。
                    menuFonts_[i].SetScale(1.0f);
                }
            }

            // --- 決定処理 ---
            if (isDecide)
            {
                // 決定音SE。
                app::core::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_Button, 2.0f);
                return true;
            }

            return false;
        }


        void TitleMenu::Render(RenderContext& rc)
        {
            // 非アクティブ状態なら描画しない。
            if (!isActive_)
                return;

            // 全項目の描画コマンドを発行
            for (int i = 0; i < (int) TitleMenuType::Max; ++i)
            {
                menuFonts_[i].Draw(rc);
            }
        }
    }
}
