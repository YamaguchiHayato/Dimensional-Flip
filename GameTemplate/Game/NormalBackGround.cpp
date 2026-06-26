// [NormalBackGround.cpp]
#include "stdafx.h"
#include "NormalBackGround.h"
#include "Src/Core/CameraManager.h"

namespace
{
    // 移植に必要な定数
    const float BG_WIDTH = 150.0f;
    const float BG_Z_OFFSET = 100.0f;
}

namespace app
{
    namespace stage
    {

        bool NormalBackGround::Start()
        {
            pCameraManager_ = FindGO<CameraManager>("cameramanager");

            // 回転（空のテクスチャが地面に向かないように）
            Quaternion rot;
            rot.SetRotationDegX(-90.0f);

            for (int i = 0; i < 10; i++)
            {
                models_[i].Init("Assets/stage/BackGround/Sky.tkm");
                models_[i].SetZprepassEnabled(false);
                models_[i].SetShadowCasterFlag(false);


                float x = static_cast<float>(i) * BG_WIDTH;
                float z = BG_Z_OFFSET + (static_cast<float>(i) * 0.1f);

                models_[i].SetPosition(x, -30.0f, z); // Y座標も調整
                models_[i].SetRotation(rot);
                models_[i].SetScale(Vector3(0.5f, 0.5f, 0.5f));

                models_[i].Update();
            }
            return true;
        }

        void NormalBackGround::Update()
        {
            for (auto& m : models_)
                m.Update();
        }


        void NormalBackGround::Render(RenderContext& rc)
        {
            if (pCameraManager_ && pCameraManager_->GetCurrentCameraMode() != CameraMode::mode2D)
                return;

            for (auto& m : models_)
                m.Draw(rc);
        }

    } // namespace stage
} // namespace app
