#pragma once
#include "Src/Actor/Stage/Gimmick/IGimmic.h"

namespace app
{
    namespace gimmick
    {
        class Block : public IGimmic
        {
        public:
            Block() = default;
            virtual ~Block() = default;

            bool Start() override;
            void Update() override;
            void Render(RenderContext& rc) override;


        public:
            void SetPos(const Vector3& pos) { gimmickPos_ = pos; }
        };
    } // namespace gimmick
} // namespace app
