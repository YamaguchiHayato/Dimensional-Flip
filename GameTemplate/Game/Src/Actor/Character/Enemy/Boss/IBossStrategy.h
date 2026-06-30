#pragma once

namespace app
{
    namespace enemy {
        class Boss;
    }
}
namespace app
{
    namespace enemyState
    {
        /**
         * @class IBossStrategy
         * @brief ボス攻撃の Strategy インターフェース。
         * @details BossAttackState が 2D/3D ごとに具象クラスを差し替えて使用する。
         */
        class IBossStrategy
        {
        public:
            /** @brief デフォルトコンストラクタ。 */
            explicit IBossStrategy() = default;

            /** @brief 仮想デストラクタ。 */
            virtual ~IBossStrategy() = default;

        public:
            /**
             * @brief 攻撃開始時の初期化。
             * @param[in] pBoss 攻撃を行うボス。
             */
            virtual void Enter(app::enemy::Boss* pBoss) = 0 {};

            /** @brief 攻撃中の更新処理。 */
            virtual void Update() = 0 {};

            /** @brief 攻撃終了時の後処理。 */
            virtual void Exit() = 0 {};

            /**
             * @brief 攻撃が完了したか判定する。
             * @return 完了していれば true。
             */
            virtual bool IsFinished() const = 0;
        };
    } // namespace enemyState
} // namespace app
