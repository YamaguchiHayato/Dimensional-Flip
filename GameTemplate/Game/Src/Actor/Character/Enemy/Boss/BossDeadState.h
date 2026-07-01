#pragma once
#include "Src/Actor/Character/Enemy/IEnemyState.h"
#include "Src/Production/Fade.h"

namespace app
{
    namespace enemy
    {
        class Boss;
    }
} // namespace app


namespace app
{
    namespace enemyState
    {
        /**
         * @class BossDeadState
         * @brief ボス撃破後の死亡ステート。フェードと BGM 停止を行う。
         */
        class BossDeadState : public IEnemyState
        {
        public:
            /**
             * @brief コンストラクタ。
             * @param[in] pBoss 制御対象のボス。
             */
            BossDeadState(app::enemy::Boss* pBoss) : pBoss_(pBoss) {}

            virtual ~BossDeadState() = default;

        public:
            /** @brief 死亡アニメーションと演出開始。 */
            void Enter() override;

            /** @brief フェード・クリア演出の更新。 */
            void Update() override;

            /** @brief 終了処理。 */
            void Exit() override;

            /**
             * @brief 遷移要求（通常は死亡で固定）。
             * @param[out] request 遷移先ステート ID。
             * @return 遷移する場合 true。
             */
            bool RequestID(uint8_t& request) override;

        private:
            app::enemy::Boss* pBoss_ = nullptr; ///< ボスへのポインタ。
            Fade* pFade_ = nullptr;             ///< フェード UI。

            bool isFadeStarted_ = false; ///< フェード開始済みか。
            bool isSceneChangeRequested_ = false;
        };
    } // namespace enemyState
} // namespace app
