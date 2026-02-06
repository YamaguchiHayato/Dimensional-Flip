#include "stdafx.h"

#include "BackGround.h"

#include "Src/Core/CameraManager.h"
#include "Src/Core/StageManager.h"
#include "../k2EngineLow/graphics/Camera.h"

namespace
{
    const float BG_Z_OFFSET = 100.0f;
    const float HEIGHT_SKY = 8.0f;
    const float HEIGHT_GROUND = -5.0f;
    const Vector3 SCALE_2D = Vector3(0.5f, 0.5f, 0.5f);

    const float BASE_HEIGHT = 25.0f;
    const float BASE_SCALE = 0.15f;

    // 山の設定
    struct MountainSetting
    {
        static constexpr float WIDTH = 800.0f;           // 画像幅
        static constexpr float SCROLL_START_X = 200.0f;  // スクロール開始位置
        static constexpr float SCROLL_SPEED = 0.5f;      // 速度係数
        static constexpr float HEIGHT_MOUNTAIN = -50.0f; // 高さ

        // カリング距離
        static constexpr float CULLING_RANGE = 1200.0f;
    };
}


namespace app
{
    namespace stage
    {
        bool BackGround::Start()
        {

            // モデルの初期化。
            SettingSky();

            pPlayer_ = FindGO<Player>("player");
            pCameraManager_ = FindGO<CameraManager>("cameramanager");

            return true;
        }


        void BackGround::Update()
        {
            for (int i = 0; i < BG_COUNT; i++)
            {
                skyModel_[i].Update();
            }
        }


        void BackGround::Render(RenderContext& rc)
        {
            if (!pCameraManager_ || pCameraManager_->GetCurrentCameraMode() != CameraMode::mode2D)
                return;

            for (int i = 0; i < BG_COUNT; i++)
            {
                skyModel_[i].Draw(rc);
            }
        }

        
        void BackGround::SettingSky()
        {
            // 回転設定。
            Quaternion rot;
            rot.SetRotationDegX(-90.0f);

            for (uint8_t i = 0; i <  BG_COUNT; i++)
            {
                // モデルの初期化。
                skyModel_[i].Init("Assets/stage/BackGround/Sky.tkm");

                // 一定間隔で並べて配置する。
                auto xPos = static_cast<float>(i) * BG_WIDTH;
                auto yPos = -30.0f;
                auto zPos = BG_Z_OFFSET + (static_cast<float>(i) * 0.1f);
                skyModel_[i].SetRotation(rot);
                skyModel_[i].SetScale(SCALE_2D);

                skyModel_[i].SetPosition(xPos, yPos, zPos);
                skyModel_[i].Update();
            }
        }
    } 
} 
