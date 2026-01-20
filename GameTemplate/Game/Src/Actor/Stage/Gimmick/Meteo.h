#pragma once
#include "Src/Actor/Stage/Gimmick/IGimmic.h"
#include "graphics/effect/EffectEmitter.h"
#include "Src/Production/AttackMarker.h"


namespace app{
    namespace production{
        class AttackMarker;
    }
}

enum class State : uint8_t
{
    Warning,
    Falling
};

namespace app
{
    namespace gimmick
    {
        class Meteo : public IGimmic
        {
        public:
            Meteo() = default;
            virtual ~Meteo() = default;

            bool Start()override;
            void Update()override;
            void Render(RenderContext& rc)override;
            inline const std::string InitGimmick(const std::string& gimmickname) override
            {
                return IGimmic::InitGimmick(gimmickname);
            };

        // セッター。
        public:
            // 目標座標をセットする。
            inline void SetTargetPos(const Vector3& target)
            {
                targetPos_ = target;
            }

            // パラメータ設定用。
            inline void SetParams(float fallSpeed, float delayTime)
            {
                fallSpeed_ = fallSpeed;
                warningTime_ = delayTime;
            }
        private:
            // 状態を更新する。
            void UpdateState();


        private:
            app::production::AttackMarker* pMarker_ = nullptr;


        private:
            State state_ = State::Warning;

            Vector3 targetPos_ = Vector3::Zero;
            Vector3 currentPos_ = Vector3::Zero;

            float timer_ = 0.0f;
            float lifeTime_ = 0.0f;
            float fallSpeed_ = 30.0f;  // 落下速度
            float warningTime_ = 2.0f; // 警告時間
        };

    }
}

