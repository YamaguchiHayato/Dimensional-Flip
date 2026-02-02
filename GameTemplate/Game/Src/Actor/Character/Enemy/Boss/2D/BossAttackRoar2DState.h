#pragma once
#include "Src/Actor/Character/Enemy/Boss/IBossStrategy.h"
#include "Src/Actor/Character/Enemy/Boss/BossType.h"

namespace app{
    namespace enemy{
        class Boss;
    }

    namespace gimmick{
        class FireBall;
    }
}

namespace app
{
    namespace enemyState
    {
        class BossAttackRoar2DState : public IBossStrategy
        {
        public:
            BossAttackRoar2DState() = default;
            virtual ~BossAttackRoar2DState() = default;


        public:
            void Enter(app::enemy::Boss* pBoss) override;
            void Update() override;
            void Exit() override;
            bool IsFinished() const override;


        // セッター。
        public:
            // 生成許可時間を設定。
            inline void SetSpawnTimer(float spawnTimer)
            {
                spawnTimer_ = spawnTimer;
            }

            // 現在のエフェクトの位置をセット。
            inline void SetStrikeIndex(uint8_t index)
            {
                strikeIndex = index;
            }

            // 現在のウェーブ数をセット。
            inline void SetWaveCount(uint8_t wave)
            {
                waveCount_ = wave;
            }


        // ゲッター。
        public:
            // 生成許可時間を取得。
            inline float GetSpawnTimer()
            {
                return spawnTimer_;
            }  

            // 現在のエフェクトの位置を取得。
            inline uint8_t GetStrikeIndex()
            {
                return strikeIndex;
            }

            // 現在のウェーブ数を取得。
            inline uint8_t GetWaveCount()
            {
                return waveCount_;
            }   

        private:
            // Playerの方に向かせる処理。
            void LookAtPlayerDirection();

            // 雷エフェクトの挙動。
            void ThunderEffectMovement();

            // エフェクトの生成位置を設定。
            void SettingThunderEffect();

        private:
            app::enemy::Boss* pBoss_ = nullptr;
            app::gimmick::FireBall* pFireBall_ = nullptr;

        private:
            float timer_ = 0.0f;
            float spawnTimer_ = 0.0f;            // 雷を出す間隔の計算用

            bool isMovingAttackStarted_ = false; // 移動攻撃を開始したか

            uint8_t strikeIndex = 0;             // 1セット内のエフェクトの位置。
            uint8_t waveCount_ = 0;              // 現在のウェーブ数。


            Quaternion rot_ = Quaternion::Identity;
            Quaternion offSetRot_ = Quaternion::Identity;
        };


    }
}

