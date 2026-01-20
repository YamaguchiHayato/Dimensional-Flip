#include "stdafx.h"

// キャラクタークラス。
#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Player/Player.h"


// 管理クラス。
#include "Src/Core/SceneManager.h"
#include "Src/Core/CameraManager.h"
#include "Src/Core/StageManager.h"
#include "Src/Core/InputManager.h"

// カメラクラス。
#include "Src/Camera/BossCameraStrategy.h"


namespace
{   
    const auto ORIGINCAMERA_EVENTTIME = 4.0f; // 演出時間
    const auto TARGET_HEIGHT = 150.0f;
    const auto SWITCH_SPEED = 10.0f;

    const Vector3 SIDE_VIEW_OFFSET = {-1500.0f, 500.0f, 0.0f};
    const Vector3 DEPTH_VIEW_OFFSET = {0.0f, 500.0f, -1500.0f};
}; 

namespace app
{
    namespace camera
    {


        bool BossCameraStrategy::Start()
        {
            pBoss_ = FindGO<app::enemy::Boss>("boss");
            if (!pBoss_)
                return false;

            // 遠くまで見えるように
            g_camera3D->SetFar(10000.0f);


            return true;
        }


        void BossCameraStrategy::Update()
        {
            if (!pBoss_ || pPlayer_)
                return;

            // クライミングカメラ。
            UpdateClimbingCamera();
        }


        void BossCameraStrategy::UpdateClimbingCamera()
        {
            // クライミング用カメラ。

            Vector3 playerPos = pPlayer_->GetPlayerPos();
            Vector3 currentPos = g_camera3D->GetPosition();

            // PlayerのY座標の高さ + オフセットに追従させる。
            Vector3 idolPos;
            idolPos.x = playerPos.x;
            idolPos.y = playerPos.y + 50.0f;
            idolPos.z = playerPos.z - 100.0f;

            // 線形保管を用いて滑らかに追従。
            Vector3 newPos = Lerp(5.0f * g_gameTime->GetFrameDeltaTime(), currentPos, idolPos);
            g_camera3D->SetPosition(newPos);

            // Playerを見る。
            g_camera3D->SetTarget(playerPos);
        }
    }
}

