#include "stdafx.h"
#include "DimensionManager.h"


namespace app
{
    namespace core
    {
        DimensionManager* DimensionManager::instance_ = nullptr;


        void DimensionManager::FlipDimension(CameraManager* pCamera, app::collision::CollisionManager& collision)
        {
            auto currentCamMode = pCamera->GetCurrentCameraMode();


            if (currentCamMode == CameraMode::mode2D)
            {
                // 3Dへ。
                pCamera->Request3DMode();
                collision.SetDimension(collision::DimensionMode::dim3D);
            }

            else if (currentCamMode == CameraMode::mode3D)
            {
                // 2Dへ。
                pCamera->Request2DMode();
                collision.SetDimension(collision::DimensionMode::dim2D);
            }
        }
    }
}
