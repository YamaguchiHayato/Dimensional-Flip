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
            // 生成時のパラメータ構造体。
            struct SpawnParam
            {
                Vector3 paramPos_;
                Vector3 paramScale;
                float paramCollision;
                bool paramFlag;


                // デフォルト値を設定。
                SpawnParam(const Vector3& pos, const Vector3& scale = Vector3::One, float radius = -1.0f, bool flag = true) : paramPos_(pos), paramScale(scale), paramCollision(radius), paramFlag(flag){}
            };


        public:
            NormalEnemy() = default;
            virtual ~NormalEnemy() = default;

        // エンジンの基本関数。
        public:
            bool Start() override;
            void Update() override;
            void Render(RenderContext& rc) override;


        // 独自の関数。
        public:
            void Press();
            void InitParam(const SpawnParam& param);
            inline const std::string InitModel(const std::string& enemyName) override
            {
                return IEnemy::InitModel(enemyName);
            };


        public:
            // 座標をセット。
            inline  void SetPos(const Vector3& pos) override
            {
                pos_ = pos;
            }


            // モデルごとに大きさを設定する。
            inline void SetScale(const Vector3& scale) override
            {
                scale_ = scale;
            }


            // 敵を踏みつけて倒せるかどうか判定する。
            inline void SetStompable(bool enable) override
            {
                isStompable_ = enable;
            }


        private:
            Vector3 initPos_ = Vector3::Zero;
            Vector3 pos_ = Vector3::Zero;
            Vector3 scale_ = Vector3(0.25f, 0.25f, 0.25f);
            Vector3 crushStartPos_ = Vector3::Zero;
            Vector3 crushStartScale_ = Vector3::Zero;
            ModelRender render_;

            bool isStompable_ = true;
            bool isCrushed_ = false;


            float radius_ = 80.0f;
            float crushScaleYRate_ = 0.2f;  // 最終Yスケール比
            float crushPosYOffset_ = 15.0f; // 下に沈める量（見た目用）

            uint8_t crushedFrame_ = 0;


        private:
            Player* pPlayer_ = nullptr;
        };

    }
}

