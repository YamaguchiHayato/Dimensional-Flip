#pragma once
#include "Src/Actor/Stage/Gimmick/IGimmic.h"

namespace app
{
    namespace gimmick
    {
        class FloatingPlatform : public IGimmic
        {
        public:
            FloatingPlatform() = default;
            virtual ~FloatingPlatform() = default;


            bool Start()override;
            void Update() override;
            void Render(RenderContext& rc) override;
            inline const std::string InitGimmick(const std::string& gimmickname) override
            {
                return IGimmic::InitGimmick(gimmickname);
            };

            // 足場を出現させる。
            void Activate(const Vector3& pos);

            // 足場を隠す。(OFF)
            inline void Deactivate()
            {
                isActive_ = false;
            }


        // セッター。
        public:
            // 足場の出現位置を設定。
            inline void SetPos(const Vector3& pos)
            {
                targetPos_ = pos;
            }

            // 足場の生存時間を設定。
            inline void SetLifeTime(float time)
            {
                lifeTime_ = time;
            }

            // 目標座標をセットする。
            inline void SetTargetPos(const Vector3& pos)
            {
                targetPos_ = pos;
            }

        private:
            CollisionObject* instanceCollision = nullptr;

        private:
            Vector3 targetPos_ = Vector3::Zero;  // 最終的な固定位置
            Vector3 currentPos_ = Vector3::Zero; // 現在の位置
            Model sharedModel_;

            bool isLoaded_ = false;
            bool isActive_ = false; // 足場がアクティブかどうか

            float timer_ = 0.0f;
            float lifeTime_ = 10.0f;             // デフォルト10秒
        };

    }
}

