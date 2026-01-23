#include "stdafx.h"
#include "Src/Core/InputManager.h"
#include "Src/Core/Game.h"

namespace app
{
    namespace core
    {
        InputManager* InputManager::instance_ = nullptr;


        void InputManager::FlipDimension(CameraManager* pCamera)
        {
            // 無効化されているなら処理は中断。
            if (!isDimensionFlip_)
                return;

            // カメラの現在のモードを取得。
            auto currentCamMode = pCamera->GetCurrentCameraMode();


            auto* findGameClass = FindGO<app::core::Game>("game");

            if (!findGameClass)
                return;


            // 2D⇔3Dの切り替え。
            // 現在のカメラモードに応じて切り替えを行う。
            // 2Dモードの場合は3Dへ、3Dモードの場合は2Dへ切り替え。   
            if (currentCamMode == CameraMode::mode2D)
            {
                // 3Dへ。
                findGameClass->ChangeDimension(CameraMode::mode3D);

            }

            else if (currentCamMode == CameraMode::mode3D)
            {
                // 2Dへ。
                findGameClass->ChangeDimension(CameraMode::mode2D);
            }
        }
    }
}
