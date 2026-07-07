#include "stdafx.h"

#include "Src/Actor/Character/Player/Player.h"
#include "Src/Presentation/UI/Components/UIPlayerHpBarView.h"

namespace
{
    // 旧 HPbarUI と同じ定数
    const auto HEART_WIDTH = 199.0f;
    const auto HEART_HEIGHT = 187.0f;
    const auto FRAME_WIDTH = 252.0f;
    const auto FRAME_HEIGHT = 23.0f;

    const Vector3 FRAME_POS{-820.0f, 450.0f, 0.0f};
    const Vector3 FONT_POS{-850.0f, 550.0f, 0.0f};

    const auto HP_FONT_SCALE = 1.5f;
    const auto OUTLINE_OFFSET = 2.0f;

    const Vector3 FONT_OFFSET[5] = {
        {OUTLINE_OFFSET, -OUTLINE_OFFSET, 0.0f},
        {-OUTLINE_OFFSET, -OUTLINE_OFFSET, 0.0f},
        {OUTLINE_OFFSET, OUTLINE_OFFSET, 0.0f},
        {-OUTLINE_OFFSET, OUTLINE_OFFSET, 0.0f},
        {0.0f, 0.0f, 0.0f},
    };

    // 旧 UIBase::InitUI と同じパス規則
    const char* kFlamePath = "Assets/UI/progressBar/flame.DDS";
    const char* kHeartPath = "Assets/UI/progressBar/Heart.DDS";
} // namespace


namespace nsApp
{
    namespace nsUI
    {
        UIPlayerHpBarView::UIPlayerHpBarView() : heartPosX_(-900.0f), heartPosY_(500.0f), spritesInitialized_(false)
        {
            InitSprites();
            InitHpFonts();
        }


        void UIPlayerHpBarView::InitSprites()
        {
            flame_.Init(kFlamePath, FRAME_WIDTH, FRAME_HEIGHT);
            flame_.SetPosition(FRAME_POS);
            flame_.SetScale(Vector3::One);
            flame_.Update();

            heart_.Init(kHeartPath, HEART_WIDTH, HEART_HEIGHT);
            heart_.SetPosition(Vector3(heartPosX_, heartPosY_, 0.0f));
            heart_.SetScale(Vector3::One);
            heart_.Update();

            spritesInitialized_ = true;
        }


        void UIPlayerHpBarView::InitHpFonts()
        {
            for (int i = 0; i < 5; ++i)
            {
                Vector3 pos = FONT_POS;
                pos.x += FONT_OFFSET[i].x;
                pos.y += FONT_OFFSET[i].y;

                hpFont_[i].SetPosition(pos);
                hpFont_[i].SetScale(HP_FONT_SCALE);
                hpFont_[i].SetColor(i < 4 ? Vector4::Black : Vector4::White);
            }
        }


        void UIPlayerHpBarView::SetPosition(float x, float y)
        {
            // GameplayHudScreen::SetupLayout からハート位置が渡される
            heartPosX_ = x;
            heartPosY_ = y;
            heart_.SetPosition(Vector3(heartPosX_, heartPosY_, 0.0f));
            heart_.Update();
        }


        void UIPlayerHpBarView::ApplyLogic(const PlayerHpBarLogic& logic)
        {
            cachedLogic_ = logic;
            UpdateFromPlayer();
        }


        void UIPlayerHpBarView::UpdateFromPlayer()
        {
            // HP テキストとハート色は Player から取得（旧 HPbarUI と同じ）
            Player* pPlayer = FindGO<Player>("player");
            if (pPlayer == nullptr)
                return;

            swprintf_s(hpText_, L"%d/%d", pPlayer->GetHP(), pPlayer->GetMaxHP());
            for (int i = 0; i < 5; ++i)
                hpFont_[i].SetText(hpText_);

            Vector4 color = Vector4::White;
            if (pPlayer->GetHP() <= 3)
                color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

            heart_.SetMulColor(color);
            hpFont_[4].SetColor(color);
        }


        void UIPlayerHpBarView::Draw(RenderContext& rc)
        {
            if (!spritesInitialized_)
                return;

            flame_.Draw(rc);
            heart_.Draw(rc);

            for (int i = 0; i < 5; ++i)
                hpFont_[i].Draw(rc);
        }
    } // namespace nsUI
} // namespace nsApp
