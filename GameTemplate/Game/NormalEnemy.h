#pragma once
#include "Src/Actor/Character/Enemy/IEnemy.h"

class Player;

enum enNormalEnemyAnimation : uint8_t
{
    ghost_Idle,
    ghost_Death,
    ghost_Num,
};

namespace app
{
    namespace enemy
    {
        class NormalEnemy : public IEnemy
        {
        public:
            NormalEnemy() = default;
            virtual ~NormalEnemy() = default;

            bool Start() override;
            void Update() override;
            void Render(RenderContext& rc) override;
            inline const  std::string InitModel(const std::string& enemyName)override
            {
                return IEnemy::InitModel(enemyName);
            };

            
            void Press();

        public:
            // セッター。
            inline void SetPos(const Vector3& pos)
            {
                pos_ = pos;
            }

            // モデルごとに大きさを設定する。
            inline void SetScale(const Vector3& scale, float radius = 80.0f)
            {
                scale_ = scale;
                radius_ = radius;
            }

            // 敵を踏みつけて倒せるかどうか判定する。
            inline void SetIsStompable(bool enable)
            {
                isStompable_ = enable;
            }
        private:
            Vector3 initPos_ = Vector3::Zero;
            Vector3 pos_ = Vector3::Zero;
            Vector3 scale_ = Vector3(0.25f, 0.25f, 0.25f);

            ModelRender bigEnemy;

            bool isStompable_ = true;
            float radius_ = 80.0f;
            bool isCrushed_ = false;
            int crushedFrame_ = 0;

            Vector3 crushStartPos_ = Vector3::Zero;
            Vector3 crushStartScale_ = Vector3::Zero;

            static constexpr int kCrushFrames = 6;      // 潰れるまでのフレーム数
            static constexpr int kCrushWaitFrames = 10; // 潰れたまま残すフレーム数
            float crushScaleYRate_ = 0.2f;              // 最終Yスケール比
            float crushPosYOffset_ = 15.0f;             // 下に沈める量（見た目用）
        private:
            Player* pPlayer_ = nullptr;
            CharacterController charaCon_;
        };

    }
}

