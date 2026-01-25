#pragma once
#include "Src/UI/BossUI/BossUIBase.h"

// ボスの攻撃予告をするUIクラス。
// BossUIManagerのヘルパークラス。

namespace app
{
    namespace nsUI
    {
        enum class BossAttackKind : uint8_t
        {
            None,
            FireBall,
            Jump,
            Meteor,
            Roar,
            Spear,
            Tumbler,
            Num,
        };


        class BossAttackIndicatorUI : public BossUIBase
        {
        public:
            void Initialize();
            void Update() override;
            void Draw(RenderContext& rc) override;


        public:
            inline void SetAttackKind(BossAttackKind type)
            {
                currentType_ = type;
            }


        private:
            SpriteRender bossAttackIcons_[(int)BossAttackKind::Num];
            BossAttackKind currentType_ = BossAttackKind::None;


        };

    }
}

