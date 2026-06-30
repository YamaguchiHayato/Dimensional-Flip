#include "stdafx.h"
#include "AttackMarker.h"

namespace nsApp
{
    namespace nsProduction
    {
        bool AttackMarker::Start()
        {
            markerRender_.Init("Assets/modelData/marker.tkm");

            return true;
        }


        void AttackMarker::Update()
        {
            // 回転演出の更新。
            RotationProduction();

            // 角度のベース。
            Quaternion baseRot = Quaternion::Identity;
            baseRot.SetRotationDegY(90.0f);

            // スピン用の回転。
            Quaternion spinRot = Quaternion::Identity;
            spinRot.SetRotationDegY(currentAngle_);

            // 回転同士を乗算。
            rotation_ = spinRot * baseRot;

            markerRender_.SetRotation(rotation_);
            markerRender_.SetPosition(position_);
            markerRender_.SetScale(scale_);
            markerRender_.Update();
        }


        void AttackMarker::Render(RenderContext& rc)
        {
            markerRender_.Draw(rc);
        }


        void AttackMarker::RotationProduction()
        {
            // 進捗率 (0.0 ～ 1.0)
            float progress = min(timer_ / maxDuration_, 1.0f);

            // 時間経過で回転スピードアップ
            float currentSpeed = 100.0f + (500.0f * progress);

            // 角度を加算（前回の Multiply ではなく、角度自体を増やしていく）
            currentAngle_ += currentSpeed * g_gameTime->GetFrameDeltaTime();
        }
    }
}
