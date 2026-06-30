#include "stdafx.h"

#include "WorldSelectUI.h"

namespace nsApp
{
    namespace nsUI
    {
        void WorldSelectUI::Init()
        {
            /**
             * @brief 各 UI パーツを初期化する
             */
            DrawStageName();
            DrawStageSelectText();
            DrawStickUI();
            DrawSelectText();
            DrawBackButtonUI();
            DrawBackText();
            DrawSkipText();
        }

        void WorldSelectUI::Update()
        {
            /**
             * @brief スプライトのアニメーションを更新する
             */
            stickUIRender_.Update();
            backButtonRender_.Update();
        }

        void WorldSelectUI::Render(RenderContext& rc)
        {
            /**
             * @brief 全 UI 要素を描画する
             */
            stageNameText_.Draw(rc);
            worldSelectTextUI_.Draw(rc);
            stickUIRender_.Draw(rc);
            selectTextUI_.Draw(rc);
            backButtonRender_.Draw(rc);
            backUIText_.Draw(rc);
        }

        void WorldSelectUI::SetStageName(const wchar_t* name)
        {
            stageNameText_.SetText(name);
        }

        void WorldSelectUI::DrawStageName()
        {
            stageNameText_.SetText(L"Stage Name");
            stageNameText_.SetPosition({-875.0f, 300.0f, 0.0f});
            stageNameText_.SetScale(1.5f);
            stageNameText_.SetColor({1.0f, 1.0f, 0.0f, 1.0f});
            stageNameText_.SetPivot({0.0f, 1.0f});
            stageNameText_.SetShadowParam(true, 2.0f, {0.0f, 0.0f, 0.0f, 1.0f});
        }

        void WorldSelectUI::DrawStageSelectText()
        {
            worldSelectTextUI_.SetText(L"Select Stage");
            worldSelectTextUI_.SetPosition({-900.0f, 500.0f, 0.0f});
            worldSelectTextUI_.SetScale(2.5f);
            worldSelectTextUI_.SetColor(Vector4::White);
            worldSelectTextUI_.SetPivot({0.5f, 0.5f});
            worldSelectTextUI_.SetShadowParam(true, 2.0f, {0.0f, 0.0f, 0.0f, 1.0f});
        }

        void WorldSelectUI::DrawStickUI()
        {
            stickUIRender_.Init("Assets/UI/button/Stick.DDS", 480, 484);
            stickUIRender_.SetPosition(Vector3(-880.0f, -450.0f, 0.0f));
            stickUIRender_.SetScale(Vector3::One * 0.5f);
            stickUIRender_.SetPivot(Vector2(0.0f, 0.0f));
        }

        void WorldSelectUI::DrawSelectText()
        {
            selectTextUI_.SetText(L"Select");
            selectTextUI_.SetPosition(Vector3(-650.0f, -300.0f, 0.0f));
            selectTextUI_.SetScale(1.5f);
            selectTextUI_.SetColor(Vector4::White);
            selectTextUI_.SetPivot({0.0f, 1.0f});
            selectTextUI_.SetShadowParam(true, 2.0f, {0.0f, 0.0f, 0.0f, 1.0f});
        }

        void WorldSelectUI::DrawBackButtonUI()
        {
            backButtonRender_.Init("Assets/UI/button/ButtonB.DDS", 1280, 706);
            backButtonRender_.SetPosition(Vector3(800.0f, -400.0f, 0.0f));
            backButtonRender_.SetScale(Vector3::One * 0.25f);
            backButtonRender_.SetPivot({1.0f, 0.5f});
        }

        void WorldSelectUI::DrawBackText()
        {
            backUIText_.SetText(L"Back");
            backUIText_.SetPosition({680.0f, -335.0f, 0.0f});
            backUIText_.SetScale(2.0f);
            backUIText_.SetColor(Vector4::White);
            backUIText_.SetPivot({1.0f, 0.5f});
            backUIText_.SetShadowParam(true, 2.0f, Vector4::Black);
        }

        void WorldSelectUI::DrawSkipText()
        {
            /**
             * @brief Skip 表記（現状は Back と同位置・未使用の可能性あり）
             */
            backUIText_.SetText(L"Skip");
            backUIText_.SetPosition({680.0f, -335.0f, 0.0f});
            backUIText_.SetScale(2.0f);
            backUIText_.SetColor(Vector4::White);
            backUIText_.SetPivot({1.0f, 0.5f});
            backUIText_.SetShadowParam(true, 2.0f, Vector4::Black);
        }

        void WorldSelectUI::TrySkip()
        {
            /**
             * @brief スキップ処理（未実装のプレースホルダ）
             */
        }
    } // namespace nsUI
} // namespace nsApp
