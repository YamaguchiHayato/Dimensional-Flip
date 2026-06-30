#include "stdafx.h"

#include "GameVersion.h"
#include "TitleInformationLayer.h"

namespace
{
    const Vector3 VER_POSITION = Vector3(800.0f, -400.0f, 0.0f);   //! Ver 表示位置
    const Vector4 BASE_COLOR = Vector4::White;                     //! 文字色
    const Vector4 SHADOW_COLOR = Vector4::Black;                   //! 影色
    const float SCALE = 0.5f;                                      //! 文字スケール
    const Vector3 COPY_POSITION = Vector3(-200.0f, -400.0f, 0.0f); //! コピーライト位置
} // namespace


namespace nsApp
{
    namespace nsTitle
    {
        bool TitleInformationLayer::Start()
        {
            /**
             * @brief Ver 表記と権利表記のフォントを初期化する
             */
            InitVersionText();
            InitCopyRightText();
            return true;
        }

        void TitleInformationLayer::Render(RenderContext& rc)
        {
            if (isVisible_ == false)
                return;

            /**
             * @brief Ver とコピーライトを描画する
             */
            versionText_.Draw(rc);
            copyRightText_.Draw(rc);
        }

        void TitleInformationLayer::InitVersionText()
        {
            versionText_.SetText(nsCore::GameVersion::GetDisplayString());
            versionText_.SetPosition(VER_POSITION);
            versionText_.SetScale(SCALE);
            versionText_.SetColor(BASE_COLOR);
            versionText_.SetPivot({1.0f, 1.0f});
            versionText_.SetShadowParam(true, 2.0f, SHADOW_COLOR);
        }

        void TitleInformationLayer::InitCopyRightText()
        {
            copyRightText_.SetText(L"C 2026 Dimension Flip Project");
            copyRightText_.SetPosition(COPY_POSITION);
            copyRightText_.SetScale(SCALE);
            copyRightText_.SetColor(BASE_COLOR);
            copyRightText_.SetShadowParam(true, 2.0f, SHADOW_COLOR);
        }
    } // namespace nsTitle
} // namespace nsApp
