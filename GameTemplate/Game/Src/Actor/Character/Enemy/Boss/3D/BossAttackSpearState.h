#pragma once
#include "Src/Actor/Character/Enemy/Boss/IBossStrategy.h"
#include "Src/Actor/Character/Enemy/Boss/Boss.h"

namespace app{
    namespace enemy{
        class Boss;
    }
}

namespace app
{
    namespace enemyState
    {
        class BossAttackSpearState : public IBossStrategy
        {
        public:
            /* コンストラクタとデストラク。*/
            BossAttackSpearState() = default;
            virtual ~BossAttackSpearState() = default;


        public:
            /**
             * @brief ボスの槍攻撃ステートに入る。
             * @param pBoss ボスのポインタ。
             */
            void Enter(app::enemy::Boss* pBoss) override;

            /**
             * @brief ボスの槍攻撃ステートを更新する。
             */
            void Update() override;

            /**
             * @brief ボスの槍攻撃ステートを終了する。
             */
            void Exit() override;


        public:
            /**
             * @brief ボスの槍攻撃ステートが終了したかどうかを判定する。
             * @return 終了している場合はtrue、そうでない場合はfalse。
             */
            inline bool IsFinished() const override
            {
                return timer_ >= 1.5f;
            }


        public:
            /**
             * @brief ボスの槍攻撃ステートの開始位置を設定する。
             * @param pos 開始位置の座標。
             */
            inline void SetStartPos(const Vector3& pos)
            {
                startPos_ = pos;
            }

        private:
            /**
             * @brief ボスの槍攻撃を生成する。
             */
            void CreateSpearAttack();


        private:
            app::enemy::Boss* pBoss_ = nullptr;         //! ボスのポインタ。


        private:
            float timer_ = 0.0f;                        //! ボスの槍攻撃ステートの経過時間。
            float centerX_ = 0.0f;                      //! ボスの槍攻撃ステートの中心座標。
            float randomOffset_ = 0.0f;                 //! ボスの槍攻撃ステートのランダムオフセット。
            float startX_ = 0.0f;                       //! ボスの槍攻撃ステートの開始座標。
            float endX_ = 0.0f;                         //! ボスの槍攻撃ステートの終了座標。
            int pattern_ = 0;                           //! ボスの槍攻撃ステートのパターン。
            bool isAttackSpawned_ = false;              //! ボスの槍攻撃ステートの攻撃が生成されたかどうか。
            Vector3 startPos_ = Vector3::Zero;          //! ボスの槍攻撃ステートの開始位置。
            Vector3 posBackStart_ = Vector3::Zero;      //! ボスの槍攻撃ステートの後方開始位置。
            Vector3 posBackTarget_ = Vector3::Zero;     //! ボスの槍攻撃ステートの後方目標位置。
            Vector3 posFrontStart_ = Vector3::Zero;     //! ボスの槍攻撃ステートの前方開始位置。
            Vector3 posFrontTarget_ = Vector3::Zero;    //! ボスの槍攻撃ステートの前方目標位置。
        };
    }
}
