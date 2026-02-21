#pragma once
#include "Src/Actor/Stage/Gimmick/IGimmic.h"
#include "Src/Production/AttackMarker.h"

namespace app {
    namespace production{
        class AttackMarker;
    }
}

class Player;

enum class SpearState : uint8_t
{
    Idle,    // 予兆。
    Rising,  // 突き上げ。
    Wait,    // 余韻。
    Retract, // 引っ込む。
};


enum class SpearType : uint8_t
{
    Vertical,    // 縦突き。 
    Cross        // Z軸方向の横突き。
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

        // ゲッター。
        public:
            // 現在の状態を取得する。
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

            // 攻撃のタイプをセットする。
            inline void SetType(SpearType type)
            {
                type_ = type;
            }

            // 座標をセットする。
            inline void SetPosition(const Vector3& position)
            {
                currentPos_ = position;
            }

            // 開始地点をセットする。
            inline void SetStartPos(const Vector3& pos)
            {
                startPos_ = pos;
            }


        private:
            // 状態を更新する。
            void UpdateState();

            // 当たり判定を処理。
            void CheckCollision();

        private:
            app::production::AttackMarker* pMarker_ = nullptr;
            Player* pPlayer_ = nullptr;

        private:
            SpearState state_ = SpearState::Idle;
            SpearType type_ = SpearType::Vertical;

            Vector3 startPos_ = Vector3::Zero;
            Vector3 targetPos_ = Vector3::Zero;
            Vector3 currentPos_ = Vector3::Zero;
            Vector3 moveDir_ = Vector3::Zero;
            Vector3 basePos_ = Vector3::Zero; 

            float timer_ = 0.0f;
            float lifeTime_ = 0.0f;

            bool hasHit_ = false;
        };

    }
}

