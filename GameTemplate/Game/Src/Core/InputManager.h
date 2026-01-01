#pragma once
// エンジン内蔵のクラスとは別の自作クラス。
#include "Src/Collision/NonCopyable.h"

#include "Src/Core/CameraManager.h"
#include "Src/Collision/CollisionManager.h"

namespace app
{
    namespace core
    {
        class InputManager : public Noncopyable
        {
        public:
            static InputManager* instance_;


        public:
            inline static InputManager* GetInstance()
            {
                return instance_;
            }
            

        public:
            void FlipDimension(CameraManager* pCamera, app::collision::CollisionManager& collision);


        private:
            InputManager() = default;
        };
    }
}
