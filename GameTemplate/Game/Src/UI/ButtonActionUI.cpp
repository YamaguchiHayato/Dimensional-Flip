#include "stdafx.h"
#include "Src/UI/ButtonActionUI.h"


namespace app
{
    namespace ui
    {
        bool ButtonActionUI::Start()
        {
            const std::string uiPath = InitUI("button/ButtonB");
            buttonRender_.Init(uiPath.c_str(), 1280.0f, 714);

            // 最初は非表示にする。
            isVisible_ = false;

            // ボタン画像の基準点を中心に設定。
            buttonRender_.SetPivot({0.5f, 0.5f});
            // ボタン画像の大きさを設定。
            return true;
        }


        void ButtonActionUI::Update()
        {
            // UIが非表示なら更新を受け付けない。
            if (!isVisible_)
                return;

            buttonRender_.SetScale(Vector3(0.25f, 0.25f, 0.25f));
            buttonRender_.Update();
        }


        void ButtonActionUI::Render(RenderContext& rc)
        {
            // 非表示なら描画しない。
            if (!isVisible_)
                return;

            // 描画。
            buttonRender_.Draw(rc);
        }


        void ButtonActionUI::ShowAt(const Vector3& worldPosition)
        {
            isVisible_ = true;

            Vector2 screenPos2D = Vector2::Zero;

            g_camera3D->CalcScreenPositionFromWorldPosition(screenPos2D, worldPosition);

            Vector3 finalScreenPos(screenPos2D.x, screenPos2D.y, 0.0f);

            finalScreenPos.y += 400.0f;

            buttonRender_.SetPosition(finalScreenPos);
        }


        void ButtonActionUI::Hide()
        {
            isVisible_ = false;
        }
    }
}
