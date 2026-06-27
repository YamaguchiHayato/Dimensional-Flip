#include "stdafx.h"

#include "Src/Actor/Stage/BackGround/BossBackGround.h"
#include "Src/Core/CameraManager.h"

namespace nsApp
{
    namespace nsStage
    {
        namespace nsBackGround
        {
            bool BossBackGround::Start()
            {
                bossModel_.Init("Assets/stage/BackGround/BossBackGround.tkm");

                bossModel_.SetPosition(0.0f, 18.0f, 200.0f);

                const auto scale = 0.073f;
                bossModel_.SetScale(scale, scale, scale);

                bossModel_.Update();

                return true;
            }

            void BossBackGround::Update()
            {
                Quaternion rot;
                rot.SetRotationDegX(-90.0f);

                bossModel_.SetRotation(rot);
                bossModel_.Update();
            }

            void BossBackGround::Render(RenderContext& rc)
            {
                auto* pCameraManager = FindGO<CameraManager>("cameramanager");
                if (pCameraManager && pCameraManager->GetCurrentCameraMode() == CameraMode::mode3D)
                    return;

                bossModel_.Draw(rc);
            }

        } // namespace nsBackGround
    } // namespace nsStage
} // namespace nspp
