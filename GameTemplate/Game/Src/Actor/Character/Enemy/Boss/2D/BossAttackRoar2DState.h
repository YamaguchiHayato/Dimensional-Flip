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
            /* コンストラクタとデストラクタ。*/
            BossAttackRoar2DState() = default;
            virtual ~BossAttackRoar2DState() = default;


        public:
            /**
             * @brief ボスの状態に入るときに呼ばれる関数。
             * @param pBoss ボスのポインタ。
             */
            void Enter(app::enemy::Boss* pBoss) override;

            /**
             * @brief ボスの状態を更新する関数。
             */
            void Update() override;

            /**
             * @brief ボスの状態から抜けるときに呼ばれる関数。
             */
            void Exit() override;

            /**
             * @brief ボスの状態が終了したかどうかを判定する関数。
             * @return 状態が終了している場合はtrue、そうでない場合はfalse。
             */
            bool IsFinished() const override;


        public:
            /**
             * @brief 生成許可時間をセット。
             * @param spawnTimer 生成許可時間。
             */
            inline void SetSpawnTimer(float spawnTimer)
            {
                spawnTimer_ = spawnTimer;
            }

            /**
             * @brief 現在のエフェクトの位置をセット。
             * @param index エフェクトの位置。
             */
            inline void SetStrikeIndex(uint8_t index)
            {
                strikeIndex = index;
            }

            /**
             * @brief 現在のウェーブ数をセット。
             * @param wave ウェーブ数。
             */
            inline void SetWaveCount(uint8_t wave)
            {
                waveCount_ = wave;
            }


        public:
            /**
             * @brief 現在の生成許可時間を取得。
             * @return 生成許可時間。
             */
            inline float GetSpawnTimer()
            {
                return spawnTimer_;
            }  

            /**
             * @brief 現在のエフェクトの位置を取得。
             * @return エフェクトの位置。
             */
            inline uint8_t GetStrikeIndex()
            {
                return strikeIndex;
            }

            /**
             * @brief 現在のウェーブ数を取得。
             * @return ウェーブ数。
             */
            inline uint8_t GetWaveCount()
            {
                return waveCount_;
            }   


        private:
            /**
             * @brief プレイヤーの方向を向く関数。
             */
            void LookAtPlayerDirection();

            /**
             * @brief 雷エフェクトの移動処理を行う関数。
             */
            void ThunderEffectMovement();

            /**
             * @brief 雷エフェクトの設定を行う関数。
             */
            void SettingThunderEffect();

        private:
            app::enemy::Boss* pBoss_ = nullptr;             //! ボスのポインタ。
            app::gimmick::FireBall* pFireBall_ = nullptr;   //! 雷エフェクトのポインタ。

        private:
            float timer_ = 0.0f;                            //! タイマー。
            float spawnTimer_ = 0.0f;                       //! 雷を出す間隔の計算用。
            bool isMovingAttackStarted_ = false;            //! 移動攻撃を開始したか。
            uint8_t strikeIndex = 0;                        //! 1セット内のエフェクトの位置。
            uint8_t waveCount_ = 0;                         //! 現在のウェーブ数。
            Quaternion rot_ = Quaternion::Identity;         //! ボスの回転。
            Quaternion offSetRot_ = Quaternion::Identity;   //! ボスの回転のオフセット。
        };
    }
}

