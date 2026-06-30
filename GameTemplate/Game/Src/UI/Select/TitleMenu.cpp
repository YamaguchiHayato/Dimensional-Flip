#include "stdafx.h"

#include "Src/Core/SoundManager.h"
#include "Src/UI/Select/TitleMenu.h"

namespace nsApp
{
    namespace nsUI
    {
        void TitleMenu::Init()
        {
            /**
             * @brief 各メニュー項目の表示テキストを設定する
             */
            menuFonts_[(int) TitleMenuType::GameStart].SetText(L"Game Start");
            menuFonts_[(int) TitleMenuType::Manual].SetText(L"Manual");
            menuFonts_[(int) TitleMenuType::GameEnd].SetText(L"Game End");

            /**
             * @brief 各項目の表示位置を設定する
             */
            menuFonts_[(int) TitleMenuType::GameStart].SetPosition({-150.0f, -100.0f, 0.0f});
            menuFonts_[(int) TitleMenuType::Manual].SetPosition({-150.0f, -200.0f, 0.0f});
            menuFonts_[(int) TitleMenuType::GameEnd].SetPosition({-150.0f, -300.0f, 0.0f});

            /**
             * @brief 全項目に共通のスケール・影・ピボットを設定する
             */
            for (uint8_t i = 0; i < (int) TitleMenuType::Max; i++)
            {
                menuFonts_[i].SetScale(2.5f);
                menuFonts_[i].SetShadowParam(true, 2.0f, {0.0f, 0.0f, 0.0f, 1.0f});
                menuFonts_[i].SetPivot(0.5f, 0.5f);
            }
        }


        bool TitleMenu::Update(bool isUp, bool isDown, bool isDecide)
        {
            /**
             * @brief 上入力でカーソルを移動する
             */
            if (isUp)
            {
                curenntIndex_--;
                if (curenntIndex_ < 0)
                    curenntIndex_ = (int) TitleMenuType::Max - 1;

                nsCore::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_CursorMove, 2.0f);
            }

            /**
             * @brief 下入力でカーソルを移動する
             */
            if (isDown)
            {
                curenntIndex_++;
                if (curenntIndex_ >= (int) TitleMenuType::Max)
                    curenntIndex_ = 0;

                nsCore::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_CursorMove, 2.0f);
            }

            /**
             * @brief 選択中項目を赤・拡大、非選択を白・通常サイズにする
             */
            for (int i = 0; i < (int) TitleMenuType::Max; ++i)
            {
                if (i == curenntIndex_)
                {
                    menuFonts_[i].SetColor(1.0f, 0.0f, 0.0f, 1.0f);
                    menuFonts_[i].SetScale(1.2f);
                }
                else
                {
                    menuFonts_[i].SetColor(1.0f, 1.0f, 1.0f, 1.0f);
                    menuFonts_[i].SetScale(1.0f);
                }
            }

            /**
             * @brief 決定入力で SE を鳴らして true を返す
             */
            if (isDecide)
            {
                nsCore::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_Button, 2.0f);
                return true;
            }

            return false;
        }


        void TitleMenu::Render(RenderContext& rc)
        {
            if (!isActive_)
                return;

            /**
             * @brief 全メニュー項目を描画する
             */
            for (int i = 0; i < (int) TitleMenuType::Max; ++i)
                menuFonts_[i].Draw(rc);
        }
    } // namespace nsUI
} // namespace nsApp
