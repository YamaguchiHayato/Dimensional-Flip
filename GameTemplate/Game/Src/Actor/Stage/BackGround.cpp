#include "stdafx.h"

#include "BackGround.h"
#include "Src/Core/CameraManager.h"
#include "Src/Core/StageManager.h"

namespace
{
    const float BG_Z_OFFSET = 100.0f;
    const Vector3 SCALE_2D = Vector3(0.5f, 0.5f, 0.5f);

    // ステージ背景の構造体。
    struct BackGroundData
    {
        const char* modelFilePath;
        float yPos = 0.0f;
        Vector3 scale = Vector3::Zero;
        float scrollSpeed = 0.0f;
        bool isLoop = true;
    };

    // 背景データリスト（Enumの順番と一致させる）
    const BackGroundData BackGroundDataList[] =
    {
        {"Assets/stage/BackGround/Sky.tkm", -30.0f, SCALE_2D, 0.1f, true},               // type_NormalStage
        {"Assets/stage/BackGround/BossBackGround.tkm", 0.0f, Vector3::One, 0.0f, false}, // type_BossBattle
    };

}


namespace app
{
    namespace stage
    {
        bool BackGround::Start()
        {
            pPlayer_ = FindGO<Player>("player");
            pCameraManager_ = FindGO<CameraManager>("cameramanager");
            return true;
        }


        void BackGround::Update()
        {
            for (int i = 0; i < BG_COUNT; i++)
            {
                backGroundModels_[i].Update();
            }
        }


        void BackGround::Render(RenderContext& rc)
        {
            if (!pCameraManager_ || pCameraManager_->GetCurrentCameraMode() != CameraMode::mode2D)
                return;

            for (int i = 0; i < BG_COUNT; i++)
            {
                backGroundModels_[i].Draw(rc);
            }
        }


        void BackGround::SetBackGroundType(EnBackGroundType type)
        {
            backGroundType_ = type;
            SettingBackGround();
        }


        void BackGround::SettingBackGround()
        {
            typeIndex_ = static_cast<int>(backGroundType_);
            dataSize_ = sizeof(BackGroundDataList) / sizeof(BackGroundDataList[0]);

            if (typeIndex_ < 0 || typeIndex_ >= dataSize_)
                typeIndex_ = 0;

            const BackGroundData& data = BackGroundDataList[typeIndex_];

            // デバッグログ
            char debugMsg[256];
            sprintf_s(debugMsg, "SettingBackGround. Type: %d, File: %s\n", typeIndex_, data.modelFilePath);
            OutputDebugStringA(debugMsg);

            rot_.SetRotationDegX(-90.0f);

            for (uint8_t i = 0; i < BG_COUNT; i++)
            {
                backGroundModels_[i].Init(data.modelFilePath);

                // 修正ポイント：BG_WIDTH(150.0f)を掛けて正しく並べる
                backGroundPosition_.x = static_cast<float>(i) * BG_WIDTH;
                backGroundPosition_.z = BG_Z_OFFSET + (static_cast<float>(i) * 0.1f);

                backGroundModels_[i].SetPosition(backGroundPosition_.x, data.yPos, backGroundPosition_.z);
                backGroundModels_[i].SetRotation(rot_);
                backGroundModels_[i].SetScale(data.scale);
                backGroundModels_[i].Update();
            }
        }
    } // namespace stage
} // namespace app
