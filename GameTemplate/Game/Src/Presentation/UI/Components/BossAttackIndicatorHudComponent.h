#pragma once

#include "Src/Presentation/UI/Components/IUIComponent.h"
#include "Src/UI/BossUI/BossAttackIndicatorUI.h"

/**
 * @file   BossAttackIndicatorHudComponent.h
 * @brief  ボス攻撃予告アイコンを描画する Component。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class BossAttackIndicatorHudComponent
         * @brief 攻撃種別に応じたアイコン表示専用 Component。
         */
        class BossAttackIndicatorHudComponent : public IUIComponent
        {
        public:
            /**
             * @brief 表示する攻撃種別を設定する。
             * @param kind 攻撃種別。None で非表示。
             */
            void SetAttackKind(BossAttackKind kind);

            /**
             * @brief 初回構築。
             */
            void OnBuild() override;

            /**
             * @brief 毎フレーム更新。
             * @param deltaTime 経過秒数。
             */
            void OnUpdate(float deltaTime) override;

            /**
             * @brief 描画。
             * @param rc 描画コンテキスト。
             * @param ownerWorld 親 Entity のワールド行列。
             */
            void OnDraw(RenderContext& rc, const Matrix& ownerWorld) override;


        private:
            SpriteRender attackIcons_[(int) BossAttackKind::Num]; //! 攻撃種別アイコンのスプライト配列。
            BossAttackKind currentKind_ = BossAttackKind::None;   //! < 現在表示中の攻撃種別。
            bool isBuilt_ = false;                                //! < 初回構築済みかどうか。
        };
    } // namespace nsUI
} // namespace nsApp
