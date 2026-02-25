#include "stdafx.h"
#include "ManualUI.h"

namespace app
{
    namespace nsUI
    {
        void ManualUI::Init()
        {
            // コントローラーの画像を初期化。
            InitManualText();

            // BボタンのUIを初期化。
            InitBackButton();

            // 「操作説明」と表記。
            InitTitleText();

            // 移動の操作説明。
            InitMoveText();

            // カメラアクションの操作説明。
            InitCameraActionText();

            // ジャンプの操作説明。
            InitJumpText();

            // 「Back」と表記
            InitBackText();

            // 最初は非表示にする。
            isActive_ = false;
        }


        void ManualUI::Update()
        {
            if (isActive_)
            {
                manualSprite_.Update();
                buttonActionUI_.Update();
            }
        }

        void ManualUI::Render(RenderContext& rc)
        {
            if (isActive_)
            {
                // コントローラー画像を描画。
                manualSprite_.Draw(rc);

                // BボタンUIを描画。
                buttonActionUI_.Draw(rc);

                // 「操作説明」と表記。
                titleFont_.Draw(rc);

                // 移動の操作説明。
                moveTextFont_.Draw(rc);

                // カメラアクションの操作説明。
                cameraActionTextFont_.Draw(rc);

                // ジャンプの操作説明。
                jumpTextFont_.Draw(rc);

                // 「Back」と表記
                backTextFont_.Draw(rc);
            }


        }


        void ManualUI::InitManualText()
        {
            // マニュアル画像を読み込む。
            manualSprite_.Init("Assets/UI/manual.DDS", 1280, 714);

            // 画面中央に配置する。
            manualSprite_.SetPosition(Vector3::Zero);
            manualSprite_.SetPivot({0.5f, 0.5f});
        }


        void ManualUI::InitBackButton()
        {
            // 画像を読み込む。
            buttonActionUI_.Init("Assets/UI/button/ButtonB.DDS", 1280, 706);
            // 座標を設定。
            buttonActionUI_.SetPosition(Vector3(800.0f, -400.0f, 0.0f));
            // 大きさを設定。
            buttonActionUI_.SetScale(Vector3::One * 0.25);
            // 基準点を設定。
            buttonActionUI_.SetPivot({1.0f, 0.5f});

        }


        void ManualUI::InitTitleText()
        {
            titleFont_.SetText(L"Operation");
            titleFont_.SetPosition(Vector3(-800.0f, 500.0f, 0.0f));
            titleFont_.SetScale(2.0f);
            titleFont_.SetPivot({0.5f, 0.5f});
            titleFont_.SetShadowParam(true, 2.0f, Vector3::Back);
        }


        void ManualUI::InitMoveText()
        {
            moveTextFont_.SetText(L"Stick\nMove");
            moveTextFont_.SetPosition(Vector3(-600.0f, 300.0f, 0.0f));
            moveTextFont_.SetScale(1.5f);
            moveTextFont_.SetPivot({0.5f, 0.5f});
            moveTextFont_.SetShadowParam(true, 2.0f, Vector3::Back);
        }


        void ManualUI::InitCameraActionText()
        {
            cameraActionTextFont_.SetText(L"B\nCamera Action");
            cameraActionTextFont_.SetPosition(Vector3(300.0f, 300.0f, 0.0f));
            cameraActionTextFont_.SetScale(1.5f);
            cameraActionTextFont_.SetPivot({0.5f, 0.5f});
            cameraActionTextFont_.SetShadowParam(true, 2.0f, Vector3::Back);
        }


        void ManualUI::InitJumpText()
        {
            jumpTextFont_.SetText(L"A\nJump");
            jumpTextFont_.SetPosition(Vector3(300.0f, 100.0f, 0.0f));
            jumpTextFont_.SetScale(1.5f);
            jumpTextFont_.SetPivot({0.5f, 0.5f});
            jumpTextFont_.SetShadowParam(true, 2.0f, Vector3::Back);
        }


        void ManualUI::InitBackText()
        {
            // 描画するテキストを設定。
            backTextFont_.SetText(L"Back");
            // テキストの座標を設定。
            backTextFont_.SetPosition(Vector3(700.0f, -340.0f, 0.0f));
            // テキストの大きさを設定。
            backTextFont_.SetScale(2.0f);
            // テキストの色を設定。
            backTextFont_.SetColor(Vector4::White);
            // テキストの基準点を設定。
            backTextFont_.SetPivot({1.0f, 0.5f});
            // テキストの影のパラメータを設定。
            backTextFont_.SetShadowParam(true, 2.0f, Vector4::Black);
        }
    } // namespace nsUI
} // namespace app
