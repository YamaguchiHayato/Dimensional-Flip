#include "stdafx.h"

#include "Src/Core/Game.h"
#include "Src/Core/InputManager.h"

namespace nsApp
{
    namespace nsCore
    {
        InputManager* InputManager::instance_ = nullptr;

        void InputManager::FlipDimension(CameraManager* pCamera)
        {
            /**
             * @brief 無効化中は処理しない
             */
            if (!isDimensionFlip_)
                return;

            /**
             * @brief 現在のカメラモードを取得
             */
            auto currentCamMode = pCamera->GetCurrentCameraMode();

            /**
             * @brief インゲーム本体 GO を検索
             */
            auto* pGame = FindGO<Game>("game");
            if (!pGame)
                return;

            /**
             * @brief 2D⇔3D を切り替え
             */
            if (currentCamMode == CameraMode::mode2D)
                pGame->ChangeDimension(CameraMode::mode3D);
            else if (currentCamMode == CameraMode::mode3D)
                pGame->ChangeDimension(CameraMode::mode2D);
        }
    } // namespace nsCore
} // namespace nsApp
