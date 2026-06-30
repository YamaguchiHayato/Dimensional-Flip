#include "stdafx.h"
#include "BossAttackIndicatorUI.h"


namespace
{
    const auto WIDTH = 1920.0f;
    const auto HEIGHT = 1080.0f;

    const auto MAGNIFICATION = 0.25f; // UIの拡大率。

    // アイコンの構造体。
    struct IconDef
    {
        app::nsUI::BossAttackKind kinds; // アイコンの種類。
        const char* uiName;             // UI名。
        float originalWIDTH;                // 元画像の幅
        float originalHEIGHT;                // 元画像の高さ
    };


    // UI名を取得。
    const IconDef ICON_DEDS[] =
    {
        { app::nsUI::BossAttackKind::FireBall, "fireBall", WIDTH, HEIGHT},
        { app::nsUI::BossAttackKind::Jump, "jumpIcon", WIDTH, HEIGHT},
        { app::nsUI::BossAttackKind::Meteor, "meteorIcon", WIDTH, HEIGHT},
        { app::nsUI::BossAttackKind::Roar, "roarIcon", WIDTH, HEIGHT},
        { app::nsUI::BossAttackKind::Spear, "spearIcon", WIDTH, HEIGHT},
        { app::nsUI::BossAttackKind::Tumbler, "tumblerIcon", WIDTH, HEIGHT},
    };
} 

namespace nsApp
{
    namespace nsUI
    {
        void BossAttackIndicatorUI::Initialize()
        {
            // 初期化。
            for (const auto& def : ICON_DEDS)
            {
                // アイコンの種類を取得。
                int index = (int) def.kinds;

                // 拡大率を設定。
                bossAttackIcons_[index].Init(FetchUIName(def.uiName).c_str(), def.originalWIDTH, def.originalHEIGHT);

                // 拡大率を一括設定。
                bossAttackIcons_[index].SetScale(Vector3(MAGNIFICATION, MAGNIFICATION, 1.0f));
            }


            // 全てのアイコン画像の中心地を0.5に設定。
            for (uint8_t i = 0; i < (uint8_t)BossAttackKind::Num; ++i ){
                bossAttackIcons_[i].SetPivot({0.5f, 0.5f});
            }
        }


        void BossAttackIndicatorUI::Update()
        {
            // 今の攻撃タイプを取得。
            if (currentType_ == BossAttackKind::None)
                return;


            // 行列の更新。
            UpdateMatrix();

            // 計算結果の行列の座標を各SpriteRenderに同期。
            const Matrix& myWorldMat = GetWorldMatrix();
            Vector3 worldPos = {
                myWorldMat.m[3][0],
                myWorldMat.m[3][1],
                myWorldMat.m[3][2]};

            // HPBarの真横に表示するように計算。
            worldPos.x -= 650.0f;
            worldPos.y -= 25.0f;

            uint8_t index = (uint8_t) currentType_;
            bossAttackIcons_[index].SetPosition(worldPos);
            bossAttackIcons_[index].Update();
        }


        void BossAttackIndicatorUI::Draw(RenderContext& rc)
        {
            if (currentType_ == BossAttackKind::None)
                return;

            bossAttackIcons_[uint8_t(currentType_)].Draw(rc);
        }


    }
}
