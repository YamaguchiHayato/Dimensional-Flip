#include "stdafx.h"
#include "Src/Camera/StageEXCameraStrategy.h"

#include "Src/Actor/Character/Player/Player.h"

namespace
{
    // アリーナ視点用にするオフセット。
    const Vector3 OFFSET(0.0f, 150.0f, -400.0f);
    const float ROTATION_SPEED = 4.0f;
}

namespace app
{
    namespace camera
    {
        StageEXCameraStrategy::StageEXCameraStrategy(Player* pPlayer)
        {
            pPlayer_ = pPlayer;
            currentRotation_ = targetRotation_;

            // 遠くまで見えるようにFarを調整。
            g_camera3D->SetFar(5000.0f);
        }

        void StageEXCameraStrategy::Update()
        {
            // --- 回転処理 ---
            float t = ROTATION_SPEED * g_gameTime->GetFrameDeltaTime();
            if (t > 1.0f)
                t = 1.0f;

            currentRotation_.Slerp(t, currentRotation_, targetRotation_);

            // --- 位置計算 ---
            const Vector3 targetPos = pPlayer_->GetPlayerPos();
            const Vector3 currentCamPos = g_camera3D->GetPosition();

            // オフセットを回転させる
            Vector3 rotatedOffset = OFFSET;
            currentRotation_.Apply(rotatedOffset);

            // 理想の位置 = プレイヤー + 回転した俯瞰オフセット
            const Vector3 idealPos = targetPos + rotatedOffset;

            // 滑らかに移動
            // FollowStrategy はグローバル名前空間にある想定です
            const float followSpeed = 5.0f * g_gameTime->GetFrameDeltaTime();
            const Vector3 newPos = Lerp(followSpeed, currentCamPos, idealPos);

            g_camera3D->SetPosition(newPos);

            // --- 注視点計算 ---
            // プレイヤーを見下ろす
            g_camera3D->SetTarget(targetPos);
        }
    } 
} 
