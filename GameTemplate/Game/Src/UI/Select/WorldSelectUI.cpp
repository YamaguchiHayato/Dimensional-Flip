#include "stdafx.h"
#include "WorldSelectUI.h"


namespace app
{
    namespace nsUI
    {
        void WorldSelectUI::Init()
        {
            // 左上にステージ名を設定
            DrawStageName();

            // シーン上部にステージセレクトと表記。
            DrawStageSelectText();

            // 左下にスティック操作のUIを描画。
            DrawStickUI();

            // 「Select」と描画。
            DrawSelectText();

            // 右下にBボタンのUIを描画。
            DrawBackButtonUI();

            // Bボタンの横にBackと描画。
            DrawBackText();

            // 右下にSkipと描画。
            DrawSelectText();
        }


        void WorldSelectUI::Update()
        {
            stickUIRender_.Update();

            backButtonRender_.Update();


        }


        void WorldSelectUI::Render(RenderContext& rc)
        {
            // ステージ名を描画。
            stageNameText_.Draw(rc);

            //「StageSelect」と描画。
            worldSelectTextUI_.Draw(rc);

            // スティックUIを描画。
            stickUIRender_.Draw(rc);

            // 「Select」と描画。
            selectTextUI_.Draw(rc);

            // バックボタンを描画。
            backButtonRender_.Draw(rc);

            // バックとテキストで描画。
            backUIText_.Draw(rc);
        }


        void WorldSelectUI::SetStageName(const wchar_t* name)
        {
            stageNameText_.SetText(name);
        }


        void WorldSelectUI::DrawStageName()
        {
            // ステージ名を設定。
            stageNameText_.SetText(L"Stage Name");
            // 座標を設定。
            stageNameText_.SetPosition({-875.0f, 300.0f, 0.0f});
            // 大きさを設定。
            stageNameText_.SetScale(1.5f);
            // 色を設定。
            stageNameText_.SetColor({1.0f, 1.0f, 0.0f, 1.0f});
            // 基準点を設定。
            stageNameText_.SetPivot({0.0f, 1.0f});
            // 影のパラメータを設定。
            stageNameText_.SetShadowParam(true, 2.0f, {0.0f, 0.0f, 0.0f, 1.0f});
        }


        void WorldSelectUI::DrawStageSelectText()
        {
            // 描画するテキストを設定。
            worldSelectTextUI_.SetText(L"Select Stage");
            // テキストの座標を設定。
            worldSelectTextUI_.SetPosition({-900.0f, 500.0f, 0.0f});
            // テキストの大きさを設定。
            worldSelectTextUI_.SetScale(2.5f);
            // テキストの色を設定。
            worldSelectTextUI_.SetColor(Vector4::White);
            // テキストの基準点を設定。
            worldSelectTextUI_.SetPivot({0.5f, 0.5f});
            // テキストの影のパラメータを設定。
            worldSelectTextUI_.SetShadowParam(true, 2.0f, {0.0f, 0.0f, 0.0f, 1.0f});

        }


        void WorldSelectUI::DrawStickUI()
        {
            // 画像を読み込む。
            stickUIRender_.Init("Assets/UI/button/Stick.DDS", 480, 484);
            // 座標を設定。
            stickUIRender_.SetPosition(Vector3(-880.0f, -450.0f, 0.0f));
            // 大きさを設定。
            stickUIRender_.SetScale(Vector3::One * 0.5f);
            // 基準点を設定。
            stickUIRender_.SetPivot(Vector2(0.0f, 0.0f));
        }


        void WorldSelectUI::DrawSelectText()
        {
            // テキストを設定。
            selectTextUI_.SetText(L"Select");
            // 座標を設定。
            selectTextUI_.SetPosition(Vector3(-650.0f, -300.0f, 0.0f));
            // 大きさを設定。
            selectTextUI_.SetScale(1.5f);
            // 色を設定。
            selectTextUI_.SetColor(Vector4::White);
            // 基準点を設定。
            selectTextUI_.SetPivot({0.0f, 1.0f});
            // 影のパラメータを設定。
            selectTextUI_.SetShadowParam(true, 2.0f, {0.0f, 0.0f, 0.0f, 1.0f});
        }


        void WorldSelectUI::DrawBackButtonUI()
        {
            // 画像を読み込む。
            backButtonRender_.Init("Assets/UI/button/ButtonB.DDS", 1280, 706);
            // 座標を設定。
            backButtonRender_.SetPosition(Vector3(800.0f, -400.0f, 0.0f));
            // 大きさを設定。
            backButtonRender_.SetScale(Vector3::One * 0.25f);
            // 基準点を設定。
            backButtonRender_.SetPivot({1.0f, 0.5f});
        }


        void WorldSelectUI::DrawBackText()
        {
            // 描画するテキストを設定。
            backUIText_.SetText(L"Back");
            // テキストの座標を設定。
            backUIText_.SetPosition({680.0f, -335.0f, 0.0f});
            // テキストの大きさを設定。
            backUIText_.SetScale(2.0f);
            // テキストの色を設定。
            backUIText_.SetColor(Vector4::White);
            // テキストの基準点を設定。
            backUIText_.SetPivot({1.0f, 0.5f});
            // テキストの影のパラメータを設定。
            backUIText_.SetShadowParam(true, 2.0f, Vector4::Black);

        }


        void WorldSelectUI::DrawSkipText()
        {
            backUIText_.SetText(L"Skip");
            backUIText_.SetPosition({680.0f, -335.0f, 0.0f});
            backUIText_.SetScale(2.0f);
            backUIText_.SetColor(Vector4::White);
            backUIText_.SetPivot({1.0f, 0.5f});
            backUIText_.SetShadowParam(true, 2.0f, Vector4::Black);
        }
    }
}
