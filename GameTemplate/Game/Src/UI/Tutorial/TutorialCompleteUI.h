#pragma once
#include "Src/UI/UIBase.h"

namespace app
{
    namespace nsUI
    {
        class TutorialCompleteUI : public UIBase
        {
        public:
            TutorialCompleteUI() = default;
            virtual ~TutorialCompleteUI() = default;


        public:
            bool Start() override;
            void Update() override;
            void Render(RenderContext& rc)override;


        public:
            // 演出開始。
            void Play();


        // ゲッター。
        public:
            // 演出が終わったかどうか判定。
            inline bool IsFinished() const
            {
                return isFinished_;
            }


        private:
            FontRender completeFont_;

            Vector3 currentPosition_ = Vector3::Zero;
            Vector3 targetPosition_ = Vector3::Zero;

            float timer_ = 0.0f;

            bool isPlaying_ = false;
            bool isFinished_ = false;

        };

    }
}
