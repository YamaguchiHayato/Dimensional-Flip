#pragma once

#include "Src/Presentation/Data/IBossHubData.h"
#include "Src/Presentation/UI/Core/UIScreen.h"

/**
 * @file   BossHudScreen.h
 * @brief  ボス戦 HUD 画面。
 */

namespace nsApp
{
    namespace nsUI
    {
        class BossStatusHudComponent;
        class BossAttackIndicatorHudComponent;

        /**
         * @class BossHudScreen
         * @brief ボスステータス + 攻撃予告 UI を構築する UIScreen。
         */
        class BossHudScreen : public UIScreen
        {
        public:
            /**
             * @brief 毎フレーム更新。
             * @param deltaTime 経過秒数。
             */
            void Update(float deltaTime) override;

            /**
             * @brief 毎フレーム描画。
             * @param rc 描画コンテキスト。
             */
            void Draw(RenderContext& rc) override;

            /**
             * @brief データソースを Bind する。
             * @param pData IBossHudData。nullptr 可。
             */
            void Bind(nsPresentation::IBossHudData* pData);

            /**
             * @brief UI ツリーを構築する。
             */
            void Build() override;

            /**
             * @brief データソースから各 Component へ値を委譲する。
             */
            void SyncFromDataSource();


        private:
            /**
             * @brief 描画をブロックするかを判定する。
             * @return 描画をブロックする場合は true、描画可能な場合は false。
             */
            bool IsDrawBlocked() const;


        private:
            nsPresentation::IBossHudData* pHudData_ = nullptr; //! データソース。nullptr 可。
            BossStatusHudComponent* pStatusComponent_ = nullptr; //! ボスステータス HUD Component。nullptr 可。
            BossAttackIndicatorHudComponent* pAttackComponent_ = nullptr; //! ボス攻撃予告 HUD Component。nullptr 可。
        };
    } // namespace nsUI
} // namespace nsApp
