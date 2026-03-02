#include "stdafx.h"
#include "BossBackGround.h"
#include "Src/Core/CameraManager.h"

namespace app
{
    namespace stage
    {
        bool BossBackGround::Start()
        {
            // ボス戦用の背景モデルを読み込む
            bossModel_.Init("Assets/stage/BackGround/BossBackGround.tkm");

            bossModel_.SetPosition(0.0f, 18.0f, 200.0f);

            auto scale = 0.073f;
            bossModel_.SetScale(scale, scale, scale);


            // 初回の行列更新
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

            // 描画
            bossModel_.Draw(rc);
        }
    } // namespace stage
} // namespace app
