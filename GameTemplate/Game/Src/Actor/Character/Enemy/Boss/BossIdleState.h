#pragma once
#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Enemy/IEnemyState.h"

namespace app
{
    namespace enemyState
    {
        /**
         * @class BossIdleState
         * @brief ボスの待機ステート。一定時間後に攻撃へ遷移する。
         */
        class BossIdleState : public IEnemyState
        {
        public:
            /**
             * @brief コンストラクタ。
             * @param[in] pBoss 制御対象のボス。
             */
            BossIdleState(app::enemy::Boss* pBoss) : pBoss_(pBoss) {}

            /** @brief デストラクタ。 */
            virtual ~BossIdleState() = default;

        public:
            /** @brief 待機アニメーション開始。 */
            void Enter() override;

            /** @brief タイマー更新と遷移判定。 */
            void Update() override;

            /** @brief 終了処理（未使用）。 */
            void Exit() override {}

            /**
             * @brief 次ステートへの遷移要求。
             * @param[out] request 遷移先ステート ID。
             * @return 遷移する場合 true。
             */
            bool RequestID(uint8_t& request) override;

        private:
            app::enemy::Boss* pBoss_ = nullptr; ///< ボスへのポインタ。
            float timer_ = 0.0f;                ///< 待機経過時間（秒）。
        };
    } // namespace enemyState
} // namespace app
