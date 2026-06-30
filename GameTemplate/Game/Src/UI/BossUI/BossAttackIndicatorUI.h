#pragma once

#include "Src/UI/BossUI/BossUIBase.h"

/**
 * @file   BossAttackIndicatorUI.h
 * @brief  ボス攻撃予告アイコン UI。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @enum BossAttackKind
         * @brief 攻撃予告アイコンの種別。
         */
        enum class BossAttackKind : uint8_t
        {
            None,     //!< 非表示
            FireBall, //!< 火球
            Jump,     //!< ジャンプ
            Meteor,   //!< 隕石
            Roar,     //!< 咆哮
            Spear,    //!< 槍
            Tumbler,  //!< 転倒攻撃
            Num
        };

        /**
         * @class BossAttackIndicatorUI
         * @brief 次のボス攻撃をアイコンで予告する HUD パーツ。
         */
        class BossAttackIndicatorUI : public BossUIBase
        {
        public:
            /* コンストラクタとデストラクタ。*/
            BossAttackIndicatorUI() = default;
            virtual ~BossAttackIndicatorUI() = default;

            /**
             * @brief 全攻撃アイコンのスプライトを読み込む。
             */
            void Initialize();

            /**
             * @brief 選択中アイコンの位置・行列を更新する。
             */
            void Update() override;

            /**
             * @brief 選択中のアイコンを描画する。
             * @param rc レンダリングコンテキスト。
             */
            void Draw(RenderContext& rc) override;

            /**
             * @brief 表示する攻撃種別を設定する。
             * @param type 攻撃種別。None で非表示。
             */
            inline void SetAttackKind(BossAttackKind type) { currentType_ = type; }

        private:
            SpriteRender bossAttackIcons_[(int) BossAttackKind::Num]; //!< 種別ごとのアイコン。
            BossAttackKind currentType_ = BossAttackKind::None;       //!< 現在表示中の種別。
        };
    } // namespace nsUI
} // namespace nsApp

using BossAttackKind = nsApp::nsUI::BossAttackKind;
using BossAttackIndicatorUI = nsApp::nsUI::BossAttackIndicatorUI;

namespace app
{
    namespace nsUI
    {
        using BossAttackKind = nsApp::nsUI::BossAttackKind;
        using BossAttackIndicatorUI = nsApp::nsUI::BossAttackIndicatorUI;
    } // namespace nsUI
} // namespace app
