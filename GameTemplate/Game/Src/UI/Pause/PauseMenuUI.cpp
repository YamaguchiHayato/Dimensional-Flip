#include "stdafx.h"

#include "PauseMenuUI.h"

namespace nsApp
{
    namespace nsUI
    {
        bool PauseMenuUI::Start()
        {
            /* poseUI。*/
            pauseSprite_.Init("Assets/UI/pose.DDS", 700.0f, 700.0f);
            pauseSprite_.SetPivot({0.5f, 0.5f});
            pauseSprite_.SetPosition(Vector3::Zero);

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
        }
    } // namespace nsUI
} // namespace nsApp
