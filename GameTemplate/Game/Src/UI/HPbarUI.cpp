#include "stdafx.h"

#include "Src/Actor/Character/Player/Player.h"
#include "Src/UI/HPbarUI.h"
#include "Src/UI/UIBase.h"

namespace
{
    const auto HEART_WIDTH = 199.0f;
    const auto HEART_HEIGHT = 187.0f;
    const auto FRAME_WIDTH = 252.0f;
    const auto FRAME_HEIGHT = 23.0f;

    const Vector3 FRAME_SCALE(Vector3::One);
    const Vector3 HEART_SCALE(Vector3::One);

    const Vector3 FLAMEPOS{-820.0f, 450.0f, 0.0f};
    const Vector3 HEARTPOS{-900.0f, 500.0f, 0.0f};
    const Vector3 LEFTPOS{-850.0f, 550.0f, 0.0f};

    const auto HP_FONT_SCALE = 1.5f;
    const auto OUTLINE_OFFSET = 2.0f;

    const Vector3 FONT_OFFSET[5] = {
        {OUTLINE_OFFSET, -OUTLINE_OFFSET, 0.0f},
        {-OUTLINE_OFFSET, -OUTLINE_OFFSET, 0.0f},
        {OUTLINE_OFFSET, OUTLINE_OFFSET, 0.0f},
        {-OUTLINE_OFFSET, OUTLINE_OFFSET, 0.0f},
        {0.0f, 0.0f, 0.0f},
    };
} // namespace

namespace nsApp
{
    namespace nsUI
    {
        bool HPbarUI::Start()
        {
            InitHPbar();
            pPlayer_ = FindGO<Player>("player");
            return true;
        }

        void HPbarUI::Update()
        {
            SetPositions();
            SetScales();
            Updates();

            if (pPlayer_)
            {
                swprintf_s(hpText_, L"%d/%d", pPlayer_->GetHP(), pPlayer_->GetMaxHP());
                for (int i = 0; i < 5; ++i)
                    hpFont_[i].SetText(hpText_);
            }

            UpdateHPColor();
        }

        void HPbarUI::Render(RenderContext& rc)
        {
            flame_.Draw(rc);
            heart_.Draw(rc);

            for (int i = 0; i < 5; ++i)
                hpFont_[i].Draw(rc);
        }

        void HPbarUI::InitHPbar()
        {
            InitHPbar_Frame();
            InitHPbar_Heart();
            InitHPbar_Life();
        }

        void HPbarUI::InitHPbar_Frame()
        {
            const std::string path = InitUI("progressBar/flame");
            flame_.Init(path.c_str(), FRAME_WIDTH, FRAME_HEIGHT);
            flame_.SetScale(FRAME_SCALE);
            flame_.SetPosition(FLAMEPOS);
            flame_.Update();
        }

        void HPbarUI::InitHPbar_Life()
        {
            for (int i = 0; i < 5; ++i)
            {
                Vector3 pos = LEFTPOS;
                pos.x += FONT_OFFSET[i].x;
                pos.y += FONT_OFFSET[i].y;

                hpFont_[i].SetPosition(pos);
                hpFont_[i].SetScale(HP_FONT_SCALE);
                hpFont_[i].SetColor(i < 4 ? Vector4::Black : Vector4::White);
            }
        }

        void HPbarUI::InitHPbar_Heart()
        {
            const std::string path = InitUI("progressBar/Heart");
            heart_.Init(path.c_str(), HEART_WIDTH, HEART_HEIGHT);
            heart_.SetScale(HEART_SCALE);
            heart_.SetPosition(HEARTPOS);
            heart_.Update();
        }

        void HPbarUI::SetPositions()
        {
            flame_.SetPosition(FLAMEPOS);
            heart_.SetPosition(HEARTPOS);
        }

        void HPbarUI::SetScales()
        {
            flame_.SetScale(FRAME_SCALE);
            heart_.SetScale(HEART_SCALE);
        }

        void HPbarUI::Updates()
        {
            flame_.Update();
            heart_.Update();
        }

        void HPbarUI::UpdateHPColor()
        {
            if (!pPlayer_)
                return;

            Vector4 color = Vector4::White;
            if (pPlayer_->GetHP() <= 3)
                color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

            heart_.SetMulColor(color);
            hpFont_[4].SetColor(color);
        }
    } // namespace nsUI
} // namespace nsApp
