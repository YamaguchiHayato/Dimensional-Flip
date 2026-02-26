#include "stdafx.h"
#include "TitleInformationLayer.h"

namespace
{
    // 座標。
    const Vector3 VER_POSITION = Vector3(800.0f, -400.0f, 0.0f);

    // ベースカラー。
    const Vector4 BASE_COLOR = { Vector4::White };

    // シャドウカラー。
    const Vector4 SHADOW_COLOR = { Vector4::Black };

    // ベーススケール。
    const float SCALE = 0.5f;

    // 座標。
    const Vector3 COPY_POSITION = Vector3(-200.0f, -400.0f, 0.0f);
}

namespace app
{
    namespace title
    {
        bool TitleInformationLayer::Start()
        {
            // Version表記テキストを初期化。
            InitVersionText();

            // 権利表記テキストを初期化する。
            InitCopyRightText();

            return true;
        }


        void TitleInformationLayer::Render(RenderContext& rc)
        {
            // Versionを描画。
            versionText_.Draw(rc);

            // 権利表記を描画。
            copyRightText_.Draw(rc);
        }


        void TitleInformationLayer::InitVersionText()
        {
            // バージョン数。
            versionText_.SetText(L"Ver 1.0.0");
            // 座標。
            versionText_.SetPosition(VER_POSITION);
            // 大きさ。
            versionText_.SetScale(SCALE);
            // ベースカラー。
            versionText_.SetColor(BASE_COLOR);
            // 中心値。
            versionText_.SetPivot({1.0f, 1.0f});
            // 影の濃さ。
            versionText_.SetShadowParam(true, 2.0f, SHADOW_COLOR);
        }


        void TitleInformationLayer::InitCopyRightText()
        {
            // 権利クレジット。
            copyRightText_.SetText(L"C 2026 Dimension Flip Project");
            // 座標。
            copyRightText_.SetPosition(COPY_POSITION);
            // 大きさ。
            copyRightText_.SetScale(SCALE);
            // ベースカラー。
            copyRightText_.SetColor(BASE_COLOR);
            // 影の濃さ。
            copyRightText_.SetShadowParam(true, 2.0f, SHADOW_COLOR);
        }
    } 
} 
