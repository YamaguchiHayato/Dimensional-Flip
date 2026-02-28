#pragma once
#include "Src/Actor/Stage/Gimmick/IGimmic.h"

class Player;

namespace app
{
    namespace gimmick
    {
        class RoarWave : public IGimmic
        {
        public:
            RoarWave() = default;
            virtual ~RoarWave();


        public:
            bool Start() override;
            void Update() override;
            void Render(RenderContext& rc) override {};


        public:
            // IGimmicの仮想関数のオーバーライド。
            inline const std::string InitGimmick(const std::string& gimmickname) override
            {
                return IGimmic::InitGimmick(gimmickname);
            };


        public:
            // エフェクトが進む進行方向と速度をセット。
            void SetParam(const Vector3& startPos, const Vector3& direction, float speed)
            {
                position_ = startPos;
                direction_ = direction;
                direction_.Normalize();
                speed_ = speed;
            }


        private:
            // 当たり判定の作成。
            bool CreateCollision();


            // エフェクトの作成。
            void CreatethunderEffect();

        private:
            Player* pPlayer_ = nullptr;
            EffectEmitter* pThunderEffect_ = nullptr;


        private:
            Vector3 position_ = Vector3::Zero;
            Vector3 playerPos_ = Vector3::Zero;
            Vector3 diff_ = Vector3::Zero;
            Vector3 direction_ = Vector3::Front;
            Vector3 playerPosition_ = Vector3::Zero;
            Vector3 diffPosition_ = Vector3::Zero;
            Vector3 currentScale_ = Vector3::One;

            bool heightCheck_ = false;
            bool isOutOfRangeZ_;
            bool isOutOfRangeX_;


            float speed_ = 0.0f;
            float hitRadius_ = 0.0f;
            float hitHeight_ = 0.0f;

        };

    }
}

