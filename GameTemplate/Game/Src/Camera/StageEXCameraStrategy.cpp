#include "stdafx.h"

#include "Src/Actor/Character/Player.h"
#include "Src/Core/StageManager.h"
#include "Src/Camera/FollowStrategy.h" 
#include "Src/Camera/StageEXCameraStrategy.h"

namespace app
{
    namespace camera
    {
        StageEXCameraStrategy::StageEXCameraStrategy(::Player* pPlayer) : pPlayer_(pPlayer)
        {
            currentRotation_ = targetRotation_;

            // 遠くまで見えるようにFarを調整。
            g_camera3D->SetFar(5000.0f);
        }

        void StageEXCameraStrategy::Update(nsK2EngineLow::Camera* camera, const float deltaTime)
        {

            if (StageManager::GetInstance()->GetCurrentStageID() != StageID::sStageEX)
                return;

            if (!pPlayer_)
                return;

            // --- 回転処理 ---
            float t = rotationSpeed_ * deltaTime;
            if (t > 1.0f)
                t = 1.0f;
            currentRotation_.Slerp(t, currentRotation_, targetRotation_);

            // --- 位置計算 ---
            const Vector3 targetPos = pPlayer_->GetPlayerPos();
            const Vector3 currentCamPos = camera->GetPosition();

            // オフセットを回転させる
            Vector3 rotatedOffset = baseOffset_;
            currentRotation_.Apply(rotatedOffset);

            // 理想の位置 = プレイヤー + 回転した俯瞰オフセット
            const Vector3 idealPos = targetPos + rotatedOffset;

            // 滑らかに移動
            // FollowStrategy はグローバル名前空間にある想定です
            const float followSpeed = 5.0f * deltaTime;
            const Vector3 newPos = ::FollowStrategy::Lerp(followSpeed, currentCamPos, idealPos);

            camera->SetPosition(newPos);

            // --- 注視点計算 ---
            // プレイヤーを見下ろす
            camera->SetTarget(targetPos);
        }
    } 
} 
