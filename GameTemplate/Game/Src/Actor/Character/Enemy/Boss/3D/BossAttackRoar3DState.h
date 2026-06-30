#pragma once
#include "Src/Actor/Character/Enemy/Boss/IBossStrategy.h"

namespace app{
    namespace enemy{
        class Boss;
    }

    namespace gimmick{
        class RoarWave;
    }
} 

namespace app
{
    namespace enemyState
    {
        class BossAttackRoar3DState : public IBossStrategy
        {
        public:
            /* コンストラクタとデストラクタ*/
            BossAttackRoar3DState() = default;
            virtual ~BossAttackRoar3DState() = default;


        public:
            /**
             * @brief 攻撃開始時の処理。
             * @param pBoss ボスのポインタ。
             */
            void Enter(app::enemy::Boss* pBoss) override;

            /**
             * @brief 攻撃中の更新処理。
             */
            void Update() override;

            /**
             * @brief 攻撃終了時の処理。
             */
            void Exit() override;

            /**
             * @brief 攻撃が終了したかどうかを判定する。
             * @return trueなら攻撃が終了している。
             */
            bool IsFinished() const override;


        private:
            /**
             * @brief 咆哮波を生成する。
             */
            void CreateRoarWave();

            /**
             * @brief プレイヤーの方向を向く処理。
             */
            void UpdateLookAtPlayer();


        private:
            app::enemy::Boss* pBoss_ = nullptr;                     //! ボスのポインタ。
            app::gimmick::RoarWave* pRoarWave_ = nullptr;           //! 咆哮波のポインタ。


        private:    
            float timer_ = 0.0f;                                    //! 攻撃の経過時間。
            float angleOffset_ = 0.0f;                              //! 攻撃の角度オフセット。
            bool isAttackSpawned_ = false;                          //! 攻撃が生成されたかどうか。
            Vector3 bossPosition_ = Vector3::Zero;                  //! ボスの座標。
            Vector3 forwardDirection_ = Vector3::Front;             //! ボスの前方向。
            Vector3 moveDirection_ = Vector3::Zero;                 //! ボスの移動方向。
            Vector3 spawnPosition_ = Vector3::Zero;                 //! 攻撃の生成座標。
            Vector3 toPlayer_ = Vector3::Zero;                      //! プレイヤーへの方向。
            Vector3 currentScale_ = Vector3::One;                   //! 攻撃の現在のスケール。
            Quaternion bossAngle_ = Quaternion::Identity;           //! ボスの角度。
            Quaternion sqredAngle_ = Quaternion::Identity;          //! ボスの角度の二乗。
            Quaternion lookAtPlayerAngle_ = Quaternion::Identity;   //! ボスのプレイヤー方向を向く角度。
        };
    } 
} 
