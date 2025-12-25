#pragma once
// エンジン内蔵のクラスとは別の自作クラス。
#include "Src/Collision/NonCopyable.h"

#include "Src/Core/CameraManager.h"
#include "Src/Collision/CollisionManager.h"

namespace app
{
    namespace core
    {
        class DimensionManager : public Noncopyable
        {
        public:
            static DimensionManager* instance_;


        public:
            inline static DimensionManager* GetInstance()
            {
                return instance_;
            }
            

        public:
            void FlipDimension(CameraManager* pCamera, app::collision::CollisionManager& collision);


        private:
            DimensionManager() = default;
        };
    }
}
