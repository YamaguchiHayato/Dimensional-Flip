#pragma once
#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Enemy/IEnemy.h"
#include "Src/Actor/Character/Enemy/IEnemyState.h"

namespace app
{
    namespace enemyState
    {
        /**
         * @class BossDamageState
         * @brief ボスの被弾ステート。HP 減少と被弾 SE を処理する。
         */
        class BossDamageState : public IEnemyState
        {
        public:
            /**
             * @brief コンストラクタ。
             * @param[in] pBoss 制御対象のボス。
             */
            BossDamageState(app::enemy::Boss* pBoss) : pBoss_(pBoss) {}

            virtual ~BossDamageState() = default;

        public:
            /** @brief 被弾アニメーションと SE 開始。 */
            void Enter() override;

            /** @brief 被弾演出の更新。 */
            void Update() override;

            void Exit() override {}

            /**
             * @brief 転倒または待機への遷移要求。
             * @param[out] request 遷移先ステート ID。
             * @return 遷移する場合 true。
             */
            bool RequestID(uint8_t& request) override;

        private:
            app::enemy::Boss* pBoss_ = nullptr; ///< ボスへのポインタ。
            bool istiredPlaying_ = false;       ///< 疲労演出再生中か。
            float timer_ = 0.0f;                ///< 被弾演出タイマー。
        };
    } // namespace enemyState
} // namespace app
