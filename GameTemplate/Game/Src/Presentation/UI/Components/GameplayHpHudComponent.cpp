#include "stdafx.h"

#include "GameplayHpHudComponent.h"
#include "Src/Actor/Character/Player/Player.h"

namespace
{
    const Vector3 FRAME_POS{-820.0f, 450.0f, 0.0f};
    const Vector3 FONT_POS{-850.0f, 550.0f, 0.0f};
    const Vector3 FONT_OFFSET[5] = {
        {2.0f, -2.0f, 0.0f}, {-2.0f, -2.0f, 0.0f}, {2.0f, 2.0f, 0.0f}, {-2.0f, 2.0f, 0.0f}, {0.0f, 0.0f, 0.0f},
    };
    const char* kFlamePath = "Assets/UI/progressBar/flame.DDS";
    const char* kHeartPath = "Assets/UI/progressBar/Heart.DDS";
} // namespace

namespace nsApp
{
    namespace nsUI
    {
        void GameplayHpHudComponent::SetHeartPosition(float x, float y)
        {
            /* ハートの表示位置を設定する。*/
            heartPosX_ = x;
            heartPosY_ = y;
        }


        void GameplayHpHudComponent::OnBuild()
        {
            /* ハートのフレームスプライトを初期化する。*/
            flame_.Init(kFlamePath, 252.0f, 23.0f);
            flame_.SetPosition(FRAME_POS);
            flame_.SetScale(Vector3::One);
            flame_.Update();

            /* ハートのスプライトを初期化する。*/
            heart_.Init(kHeartPath, 199.0f, 187.0f);
            heart_.SetPosition(Vector3(heartPosX_, heartPosY_, 0.0f));
            heart_.SetScale(Vector3::One);
            heart_.Update();

            /* ハートの HP テキストを初期化する。*/
            for (int i = 0; i < 5; ++i)
            {
                Vector3 pos = FONT_POS;
                pos.x += FONT_OFFSET[i].x;
                pos.y += FONT_OFFSET[i].y;
                hpFont_[i].SetPosition(pos);
                hpFont_[i].SetScale(1.5f);
                hpFont_[i].SetColor(i < 4 ? Vector4::Black : Vector4::White);
            }

            spritesInitialized_ = true;
        }


        void GameplayHpHudComponent::OnUpdate(float deltaTime)
        {
            /* 前フレームからの経過時間は使用しない。*/
            (void) deltaTime;
            UpdateFromPlayer();
        }


        void GameplayHpHudComponent::UpdateFromPlayer()
        {
            /* プレイヤーの HP を取得し、テキスト表示を更新する。*/
            Player* pPlayer = FindGO<Player>("player");
            if (pPlayer == nullptr)
                return;

            /* HP テキストを更新する。*/
            swprintf_s(hpText_, L"%d/%d", pPlayer->GetHP(), pPlayer->GetMaxHP());
            for (int i = 0; i < 5; ++i)
                hpFont_[i].SetText(hpText_);

            /* HP が 3 以下の場合は赤色にする。*/
            if (pPlayer->GetHP() <= 3)
                color_ = Vector4::Red;

            heart_.SetMulColor(color_);
            hpFont_[4].SetColor(color_);
        }


        void GameplayHpHudComponent::OnDraw(RenderContext& rc, const Matrix& ownerWorld)
        {
            /* 親 Entity のワールド行列は使用しない。*/
            (void) ownerWorld;

            /* スプライトが初期化されていない場合は描画しない。*/
            if (!spritesInitialized_)
                return;

            /* HP 枠・ハート・数値を描画する。*/
            flame_.Draw(rc);
            heart_.Draw(rc);

            /* HP テキストを描画する。*/
            for (int i = 0; i < 5; ++i)
                hpFont_[i].Draw(rc);
        }
    } // namespace nsUI
} // namespace nsApp
