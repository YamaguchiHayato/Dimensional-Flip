#pragma once
#include "Src/Actor/Character/Enemy/Boss/IBossStrategy.h"
#include "Src/Actor/Character/Enemy/Boss/BossType.h"
#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Stage/Gimmick/BossGimmick/FireBall.h"

#include "Src/Actor/Character/Player/Player.h"

namespace app
{
    namespace enemyState
    {
        /**
         * @enum AttackStep 
         * @brief ボスの火の玉攻撃状態のステップ。
         */
        enum class AttackStep : uint8_t
        {
            Breath,  //! 口元から火を吹く（溜め/予兆）
            Barrage, //! 画面奥から火の玉が降ってくる
            Finish   //! 攻撃終了。
        };


        class BossAttackFireBallState : public IBossStrategy
        {
        public:
            /* コンストラクタとデストラクタ。*/
            BossAttackFireBallState() = default;
            virtual ~BossAttackFireBallState() = default;


        public:
            /**
             * @brief ボスの攻撃状態に入る。
             * @param pBoss ボスのポインタ。
             */
            void Enter(app::enemy::Boss* pBoss)override;

            /**
             * @brief ボスの攻撃状態を更新する。
             */
            void Update() override;

            /**
             * @brief ボスの攻撃状態を終了する。
             */
            void Exit() override;

            /**
             * @brief ボスの攻撃状態が終了したかどうかを返す。
             * @return 終了している場合はtrue、そうでない場合はfalse。
             */
            bool IsFinished() const override;


        public:
            /**
             * @brief 火の玉の発射方向をセットする。
             * @param direction 発射方向のベクトル。
             */
            inline void SetFireBallDirection(const Vector3& direction)
            {
                moveDirection_ = direction;
            }

            /**
             * @brief ボスの攻撃状態の現在のステップをセットする。
             * @param step 現在のステップ。
             */
            inline void SetCurrentStep(AttackStep step)
            {
                currentStep_ = step;
                stepTimer_ = 0.0f;
            }

            /**
             * @brief ボスの攻撃状態の初期値をセットする。
             * @param pBoss ボスのポインタ。
             */
            inline void SetInitialValue(app::enemy::Boss* pBoss)
            {
                pBoss_ = pBoss;
                timer_ = 0.0f;
                shootTimer_ = 0.0f;
            }


        private:
            /**
             * @brief ブレス攻撃のステップを更新する。
             * @param dTime デルタタイム。
             */
            void UpdateBreastStep(float dTime);

            /**
             * @brief 火の玉を発射する。
             */
            void ShotFireBall();

            /**
             * @brief ブレス攻撃の更新処理。
             */
            void UpdateBreath();

            /**
             * @brief バラ撒き攻撃の更新処理。
             * @param dTime デルタタイム。
             */
            void UpdateBarrage(float dTime);


        private:
            /**
             * @brief プレイヤーの方向を向く処理。
             */
            void LookAtPlayerDirection();


        private:
            app::enemy::Boss* pBoss_ = nullptr;               //! ボスのポインタ。
            app::gimmick::FireBall* pFireBall_ = nullptr;     //! 火の玉のポインタ。
            app::gimmick::FireBall* pBreathEffect_ = nullptr; //! ブレスエフェクトのポインタ。

            Player* pPlayer = nullptr;                        //! プレイヤーのポインタ。

        private:
            AttackStep currentStep_ = AttackStep::Breath;     //! 現在の攻撃ステップ。

            Vector3 spawnPos_ = Vector3::Zero;                //! 火の玉のスポーン位置。
            Vector3 targetDir = Vector3::Right;               //! 火の玉の目標方向。
            Vector3 moveDirection_ = Vector3::Zero;           //! 火の玉の移動方向。

            Quaternion rotation_ = Quaternion::Identity;      //! 火の玉の回転。
            Quaternion offSet_ = Quaternion::Identity;        //! 火の玉の回転オフセット。

            float timer_ = 0;                                 //! 攻撃ステップのタイマー。
            float shootTimer_ = 0;                            //! 火の玉を発射するタイマー。
            float currentAngle_ = 0.0f;                       //! 現在の角度。
            float randomAngle_ = 0.0f;                        //! ばらつかせる角度。
            float angleOffset_ = 0.0f;                        //! 角度オフセット。
            float finalAngle_ = 0.0f;                         //! 最終的な角度。
            float stepTimer_ = 0.0f;                          //! 攻撃ステップのタイマー。
            float moveSpeed_ = 0.0f;                          //! 火の玉の移動速度。
        };
    }
}

