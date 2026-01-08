#pragma once
#include "Src/UI/UIBase.h"



namespace app
{
    namespace ui
    {
        class ButtonActionUI : public UIBase
        {
        public:
            ButtonActionUI() = default;
            virtual ~ButtonActionUI() = default;


        public:
            bool Start() override;
            void Update()override;
            void Render(RenderContext& rc)override;
            inline const std::string InitUI(const std::string& UIname) override
            {
                return UIBase::InitUI(UIname);
            };


        public:
            // worldPosition : Playerの座標。
            void ShowAt(const Vector3& worldPosition);
            // 非表示にする。
            void Hide();


        private:
            SpriteRender buttonRender_;


        private:
            bool isVisible_ = false;
        };

    }
}
