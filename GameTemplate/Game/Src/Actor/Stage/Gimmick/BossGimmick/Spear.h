#pragma once
#include "Src/Actor/Stage/Gimmick/IGimmic.h"
#include "Src/Production/AttackMarker.h"

namespace app {
    namespace production{
        class AttackMarker;
    }
}


enum class SpearState : uint8_t
{
    Idle,    // 予兆。
    Rising,  // 突き上げ。
    Wait,    // 余韻。
    Retract, // 引っ込む。
};

namespace app
{
    namespace gimmick
    {
        class Spear : public IGimmic
        {
        public:
            Spear() = default;
            virtual ~Spear() = default;

            bool Start() override;
            void Update() override;
            void Render(RenderContext& rc)override;
            inline const std::string InitGimmick(const std::string& gimmickname) override
            {
                return IGimmic::InitGimmick(gimmickname);
            };

        public:
            // 目標座標をセットする。
            inline void SetTargetPos(const Vector3& pos)
            {
                targetPos_ = pos;
            }


        private:
            // 状態を更新する。
            void UpdateState();


        private:
            app::production::AttackMarker* pMarker_ = nullptr;


        private:
            SpearState state_ = SpearState::Idle;

            Vector3 targetPos_ = Vector3::Zero;
            Vector3 currentPos_ = Vector3::Zero;
            float timer_ = 0.0f;
            float lifeTime_ = 0.0f;
        };

    }
}

