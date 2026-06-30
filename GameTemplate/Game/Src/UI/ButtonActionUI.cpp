#include "stdafx.h"

#include "Src/UI/ButtonActionUI.h"

namespace nsApp
{
    namespace nsUI
    {
        bool ButtonActionUI::Start()
        {
            const std::string uiPath = InitUI("button/ButtonB");
            buttonRender_.Init(uiPath.c_str(), 1280.0f, 714);
            isVisible_ = false;
            buttonRender_.SetPivot({0.5f, 0.5f});
            return true;
        }

        void ButtonActionUI::Update()
        {
            if (!isVisible_)
                return;

            buttonRender_.SetScale(Vector3(0.25f, 0.25f, 0.25f));
            buttonRender_.Update();
        }

        void ButtonActionUI::Render(RenderContext& rc)
        {
            if (!isVisible_)
                return;

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
    } // namespace nsUI
} // namespace nsApp
