#pragma once
#include "IBackGround.h"

namespace app
{
    namespace stage
    {
        class BossBackGround : public IBackGround
        {
        public:
            BossBackGround() = default;
            virtual ~BossBackGround() = default;


        public:
            bool Start() override;
            void Update() override;
            void Render(RenderContext& rc) override;


        private:
            // ボス戦は1枚の巨大なモデルで覆う想定
            ModelRender bossModel_;
        };
    } 
} 
