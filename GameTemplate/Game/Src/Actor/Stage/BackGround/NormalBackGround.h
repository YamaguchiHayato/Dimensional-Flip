#pragma once
#include "IBackGround.h"
#include "Src/Core/CameraManager.h"

namespace app
{
    namespace stage
    {
        class NormalBackGround : public IBackGround
        {
        public:
            NormalBackGround() = default;
            virtual ~NormalBackGround() = default;


        public:
            bool Start() override;
            void Update() override;
            void Render(RenderContext& rc) override;


        public:
            // 座標を上書き。
            inline void SetOverrideTrackingPosition(const Vector3& overridePosition)
            {
                isOverrideActive_ = true;
                overrideTrackingPosition_ = overridePosition;
            }

            // 上書きを解除。
            inline void ClearOverride()
            {
                isOverrideActive_ = false;
            }


        private:
            CameraManager* pCameraManager_ = nullptr;

        private:
            ModelRender models_[10];

            Vector3 overrideTrackingPosition_;

            bool isOverrideActive_ = false;
        };

    }
}
