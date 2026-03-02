#pragma once

namespace app
{
    namespace stage
    {
        class IBackGround : public IGameObject
        {
        public:
            IBackGround() = default;
            virtual ~IBackGround() = default;


        public:
            virtual bool Start() override = 0;
            virtual void Update() override = 0;
            virtual void Render(RenderContext& rc) override = 0;
        };

    }
}
