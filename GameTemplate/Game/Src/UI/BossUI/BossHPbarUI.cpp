#include "stdafx.h"
#include "Src/UI/BossUI/BossHPbarUI.h"

namespace
{
    // ボスアイコンのサイズ。
    const auto ICON_WIDHT = 350.0f;
    const auto ICON_HEIGHT = 250.0f;


    // HPバーフレームのサイズ。
    const auto FRAME_WIDTH = 600.0;
    const auto FRAME_HEIGHT =  600.0f;


    // HPバーCurrentのサイズ。
    const auto BAR_WIDTH = 520.0f;
    const auto BAR_HEIGHT = 95.0f;


    // HPバーDamageのサイズ。
    const auto DAMAGE_BAR_WIDTH = 550.0f;
    const auto DAMAGE_BAR_HEIGHT = 445.0f;

}

namespace app
{
    namespace nsUI
    {
        void BossHPbarUI::Initialize()
        {

            // 各スプライトの初期化。
            // ボスアイコン。
            bossIconRender_.Init(FetchUIName("bossIcon").c_str(), ICON_WIDHT, ICON_HEIGHT);

            // HPバーフレーム。
            hpBarFrameRender_.Init(FetchUIName("hpBar_flame").c_str(), FRAME_WIDTH, FRAME_HEIGHT);

            // 現在HPバー。
            hpBarCurrentBarRender_.Init(FetchUIName("hpBar_current").c_str(), BAR_WIDTH, BAR_HEIGHT);

            // ダメージHPバー。
            hpBarDamageBarRender_.Init(FetchUIName("hpBar_damage").c_str(), DAMAGE_BAR_WIDTH, DAMAGE_BAR_HEIGHT);


            // 基準点を設定。
            hpBarCurrentBarRender_.SetPivot({0.0f, 0.5f});
            transform_.localScale_ = Vector3::One;
        }


        void BossHPbarUI::Update()
        {
            // 行列の更新。
            UpdateMatrix();

            // HPbarアニメーション。
            // currentBar → 即座に、
            // damageBar → 徐々に減少。
            UpdateHPbarAnimation();


            //　FinalRound時にのみ演出。
            FinalRoundHPbarAnimation();


            // 計算結果の行列の座標を各SpriteRenderに同期。
            const Matrix& myWorldMat = GetWorldMatrix();
            Vector3 worldPos = {
                myWorldMat.m[3][0],
                myWorldMat.m[3][1],
                myWorldMat.m[3][2]
            };

            // BossIconの真横にHPbarを表示するように計算。
            Vector3 iconsPos = worldPos;
            iconsPos.x -= 330.0f;

            Vector3 barInsidePos = worldPos;
            barInsidePos.x -= 280.0f; // 枠の「ふち」の厚み分だけ右へ


            bossIconRender_.SetPosition(iconsPos);
            hpBarFrameRender_.SetPosition(worldPos);
            hpBarCurrentBarRender_.SetPosition(barInsidePos);
            hpBarDamageBarRender_.SetPosition(worldPos);


            bossIconRender_.Update();
            hpBarFrameRender_.Update();
            hpBarCurrentBarRender_.Update();
            hpBarDamageBarRender_.Update();
        }


        void BossHPbarUI::UpdateHPbarAnimation()
        {

            if (damagePercent_ > currentPercent_)
            {
                damagePercent_ -= 0.005f;

                if (damagePercent_ , currentPercent_)
                    damagePercent_ = currentPercent_;
            }

            else
                damagePercent_ = currentPercent_;
        }


        void BossHPbarUI::FinalRoundHPbarAnimation()
        {
            if (hpbarState_ == HPbarState::FinalPhase)
            {
                transform_.localPosition_.x += (rand() % 10 - 5) * 0.1f;
                transform_.localPosition_.y += (rand() % 10 - 5) * 0.1f;
            }
        }


        void BossHPbarUI::Draw(RenderContext& rc)
        {
            // 親クラスから計算されたワールド行列を取得。
            const Matrix& worldMatrix = GetWorldMatrix();

            // ワールド行列から座標を抽出。
            Vector3 worldPos;
            worldPos.x = worldMatrix.m[3][0];
            worldPos.y = worldMatrix.m[3][1];
            worldPos.z = worldMatrix.m[3][2];

            Vector3 iconsPos = worldPos;
            iconsPos.x -= 350.0f;

            // --- 描画順序：奥から手前へ ---

            hpBarDamageBarRender_.Draw(rc);

            // カレント
            hpBarCurrentBarRender_.SetScale({currentPercent_, 1.0f, 1.0f});
            hpBarCurrentBarRender_.Draw(rc);

            // フレーム
            hpBarFrameRender_.Draw(rc);

            // アイコン
            bossIconRender_.SetPosition(iconsPos);
            bossIconRender_.Draw(rc);
        }
    }
}
