#include "stdafx.h"

#include "ManualUI.h"

namespace nsApp
{
    namespace nsUI
    {
        void ManualUI::Init()
        {
            /**
             * @brief 各パーツを初期化する
             */
            InitManualText();
            InitGuideLine();
            InitBackButton();
            InitTitleText();
            InitMoveText();
            InitCameraActionText();
            InitJumpText();
            InitBackText();

            isActive_ = false; //! 初期は非表示
        }

        void ManualUI::Update()
        {
            if (!isActive_)
                return;

            /**
             * @brief 表示中スプライトのアニメーションを更新する
             */
            manualSprite_.Update();
            moveLine_.Update();
            jumpLine_.Update();
            cameraActionLine_.Update();
            buttonActionUI_.Update();
        }

        void ManualUI::Render(RenderContext& rc)
        {
            if (!isActive_)
                return;

            /**
             * @brief 画像・ガイドライン・テキストを順に描画する
             */
            manualSprite_.Draw(rc);
            moveLine_.Draw(rc);
            jumpLine_.Draw(rc);
            cameraActionLine_.Draw(rc);
            buttonActionUI_.Draw(rc);
            titleFont_.Draw(rc);
            moveTextFont_.Draw(rc);
            cameraActionTextFont_.Draw(rc);
            jumpTextFont_.Draw(rc);
            backTextFont_.Draw(rc);
        }

        void ManualUI::InitManualText()
        {
            /**
             * @brief コントローラー画像を画面中央に配置する
             */
            manualSprite_.Init("Assets/UI/manual.DDS", 1280, 714);
            manualSprite_.SetPosition(Vector3::Zero);
            manualSprite_.SetPivot({0.5f, 0.5f});
        }

        void ManualUI::InitGuideLine()
        {
            const char* linePath = "Assets/UI/showController.DDS"; //! ガイドライン用テクスチャ

            /**
             * @brief 移動（スティック）用ガイドラインを配置する
             */
            moveLine_.Init(linePath, 800, 300.0f);
            moveLine_.SetPosition(Vector3(80.0f, 80.0f, 0.0f));
            moveLine_.SetScale(Vector3::One * 3.0f);
            moveLine_.SetMulColor(Vector4::Black);
            moveLine_.SetPivot({0.5f, 0.5f});

            /**
             * @brief ジャンプ（A ボタン）用ガイドラインを配置する
             */
            jumpLine_.Init(linePath, 800, 300.0f);
            jumpLine_.SetPosition(Vector3(550.0f, 350.0f, 0.0f));
            Quaternion rotJump = Quaternion::Identity; //! ジャンプ線の回転
            rotJump.AddRotationDegZ(90.0f);
            jumpLine_.SetRotation(rotJump);
            jumpLine_.SetScale(Vector3::One * 3.0f);
            jumpLine_.SetMulColor(Vector4::Black);
            jumpLine_.SetPivot({0.5f, 0.5f});

            /**
             * @brief カメラ操作（B ボタン）用ガイドラインを配置する
             */
            cameraActionLine_.Init(linePath, 800, 300.0f);
            cameraActionLine_.SetPosition(Vector3(550.0f, 220.0f, 0.0f));
            Quaternion rotCam = Quaternion::Identity; //! カメラ線の回転
            rotCam.AddRotationDegZ(70.0f);
            cameraActionLine_.SetRotation(rotCam);
            cameraActionLine_.SetScale(Vector3::One * 3.0f);
            cameraActionLine_.SetMulColor(Vector4::Black);
            cameraActionLine_.SetPivot({0.5f, 0.5f});
        }

        void ManualUI::InitBackButton()
        {
            /**
             * @brief 右下に B ボタンアイコンを配置する
             */
            buttonActionUI_.Init("Assets/UI/button/ButtonB.DDS", 1280, 706);
            buttonActionUI_.SetPosition(Vector3(800.0f, -400.0f, 0.0f));
            buttonActionUI_.SetScale(Vector3::One * 0.25f);
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
            backTextFont_.SetText(L"Back");
            backTextFont_.SetPosition(Vector3(700.0f, -340.0f, 0.0f));
            backTextFont_.SetScale(2.0f);
            backTextFont_.SetColor(Vector4::White);
            backTextFont_.SetPivot({1.0f, 0.5f});
            backTextFont_.SetShadowParam(true, 2.0f, Vector4::Black);
        }
    } // namespace nsUI
} // namespace nsApp
