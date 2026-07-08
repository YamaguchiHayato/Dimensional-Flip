#pragma once

#include "IBossHubData.h"

namespace app
{
    namespace enemy
    {
        class Boss;
    }
} // namespace app

namespace nsApp
{
    namespace nsUI
    {
        class BossHudScreen;
    }

    namespace nsPresentation
    {
        /**
         * @class BossHudData
         * @brief ボス本体と BossHudScreen の仲介（旧 BossUIManager のデータ責務）。
         */
        class BossHudData : public IBossHudData
        {
        public:
            /**
             * @brief 更新先 Screen を登録する。
             * @param pScreen BossHudScreen。nullptr 可。
             */
            void SetScreen(nsUI::BossHudScreen* pScreen);

            /**
             * @brief 監視対象ボスを登録する。
             * @param pBoss ボス本体。nullptr 可。
             */
            void SetBoss(app::enemy::Boss* pBoss);

            /**
             * @brief 現在HPを取得。
             * @return 現在HP。
             */
            float GetCurrentHp() const override;

            /**
             * @brief 最大HPを取得。
             * @return 最大HP。
             */
            float GetMaxHp() const override;

            /**
             * @brief HPの割合を取得。
             * @return 0.0f〜1.0f。
             */
            float GetHpRatio() const override;

            /**
             * @brief 攻撃予告アイコン種別を取得。
             * @return 攻撃種別。
             */
            nsUI::BossAttackKind GetAttackKind() const override;

            /**
             * @brief 保持データを Screen へ反映する。
             */
            void SyncToScreen();


        private:
            nsUI::BossHudScreen* pScreen_ = nullptr; //! < 反映先 Screen。
            app::enemy::Boss* pBoss_ = nullptr;      //! < 監視対象ボス。
        };
    } // namespace nsPresentation
} // namespace nsApp
