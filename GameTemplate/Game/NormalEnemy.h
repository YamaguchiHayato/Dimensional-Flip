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

            


        public:
            // セッター。
            inline void SetPos(const Vector3& pos)
            {
                pos_ = pos;
            }

        private:
            Vector3 initPos_ = Vector3::Zero;
            Vector3 pos_ = Vector3::Zero;

            float radius_ = 80.0f;

        private:
            Player* pPlayer_ = nullptr;
        };

    }
}

