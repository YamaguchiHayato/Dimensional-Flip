#pragma once
#include "Src/Actor/Stage/Gimmick/IGimmic.h"

namespace app
{
    namespace gimmick
    {
        class TargetWall : public IGimmic
        {
        public:
            TargetWall() = default;
            virtual ~TargetWall() = default;

            bool Start() override;
            void Update() override;
            void Render(RenderContext& rc) override;

            // 条件達成時に呼ぶ関数（壁を消す）
            void Vanish();


        public:
            void SetPos(const Vector3& pos) { gimmickPos_ = pos; }


        private:
            bool isActive_ = true; // 壁が存在しているか？
        };

    } // namespace gimmick
} // namespace app
