#pragma once
// エンジン内蔵のクラスとは別の自作クラス。
#include "Src/Collision/NonCopyable.h"

#include "Src/Core/CameraManager.h"

namespace app
{
    namespace core
    {
        class InputManager : public Noncopyable
        {
        public:
            static InputManager* instance_;


        public:
            // Instanceの取得。
            inline static InputManager* GetInstance()
            {
                if (instance_ == nullptr)
                    instance_ = new InputManager();
                return instance_;
            }

            // Instanceの破棄。
            inline static void DeleteInstance()
            {
                if (instance_)
                {
                    delete instance_;
                    instance_ = nullptr;
                }
            }


        // セッター。
        public:
            inline void SetDimensionFlipFlag(bool flag)
            {
                isDimensionFlip_ = flag;
            }


        public:
            void FlipDimension(CameraManager* pCamera);


        private:
            InputManager() = default;


        private:
            bool isDimensionFlip_ = true;
        };
    }
}
