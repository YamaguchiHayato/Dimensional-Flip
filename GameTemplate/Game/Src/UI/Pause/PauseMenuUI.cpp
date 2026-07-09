#include "stdafx.h"

#include "PauseMenuUI.h"

namespace
{
    const auto FONT_SIZE = 1.5f;
}

namespace nsApp
{
    namespace nsUI
    {
        bool PauseMenuUI::Start()
        {
            /* poseUI。*/
            pauseSprite_.Init("Assets/UI/pose.DDS", 500.0f, 500.0f);
            pauseSprite_.SetPivot({0.5f, 0.5f});
            pauseSprite_.SetPosition(Vector3::Zero);

            /* ReStart UI。*/
            restartFont_.SetText(L"つづき");
            restartFont_.SetScale(FONT_SIZE);
            restartFont_.SetColor({1, 1, 1, 1});
            restartFont_.SetPosition({-200.0f, 140.0f, 0.0f});

            /* ワールドセレクト画面に戻る。*/
            returnWorld_.SetText(L"ステージ");
            returnWorld_.SetScale(FONT_SIZE);
            returnWorld_.SetColor({1, 1, 1, 1});
            returnWorld_.SetPosition({-200.0f, 20.0f, 0.0f});

            /* サウンド設定画面。*/
            returnTitle_.SetText(L"せってい");
            returnTitle_.SetScale(FONT_SIZE);
            returnTitle_.SetColor({1, 1, 1, 1});
            returnTitle_.SetPosition({-200.0f, -120.0f, 0.0f});

            /* 開閉フラグ。*/
            isOpen_ = false;
            return true;
        }


        void PauseMenuUI::Update()
        {
            /* 開いていなければ処理をしない。*/
            if (!isOpen_)
                return;

            /* 画像の更新。*/
            pauseSprite_.Update();
        }


        void PauseMenuUI::Render(RenderContext& rc)
        {
            if (!isOpen_)
                return;

            /* 描画。*/
            pauseSprite_.Draw(rc);
            restartFont_.Draw(rc);
            returnWorld_.Draw(rc);
            returnTitle_.Draw(rc);
        }


        void PauseMenuUI::SetSelectedIndex(int index)
        {
            selectedIndex_ = index;
            ApplySelectionVisual();
        }


        void PauseMenuUI::ApplySelectionVisual()
        {
            const Vector4 normal = {1, 1, 1, 1};
            const Vector4 active = {1, 0.9f, 0.2f, 1};
            restartFont_.SetColor(selectedIndex_ == 0 ? active : normal);
            returnWorld_.SetColor(selectedIndex_ == 1 ? active : normal);
            returnTitle_.SetColor(selectedIndex_ == 2 ? active : normal);
        }
    } // namespace nsUI
} // namespace nsApp
