#pragma once

#include "Src/UI/BossUI/BossUIBase.h"

/**
 * @file   BossHPbarUI.h
 * @brief  ボス HP バー UI。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @enum HPbarState
         * @brief HP バーの演出モード。
         */
        enum class HPbarState
        {
            Normal,     //!< 通常表示
            FinalPhase, //!< 最終フェーズ（揺れ演出）
        };

        /**
         * @class BossHPbarUI
         * @brief ボスアイコン・枠・現在 HP・遅延ダメージバーを描画する。
         */
        class BossHPbarUI : public BossUIBase
        {
        public:
            /* コンストラクタとデストラクタ。*/
            BossHPbarUI() = default;
            virtual ~BossHPbarUI() = default;

            /**
             * @brief 4 枚のスプライト（アイコン・枠・現在・ダメージ）を初期化する。
             */
            void Initialize();

            /**
             * @brief HP 割合アニメーションとスプライト位置を更新する。
             */
            void Update() override;

            /**
             * @brief HP バーを奥から手前の順で描画する。
             * @param rc レンダリングコンテキスト。
             */
            void Draw(RenderContext& rc) override;

            /**
             * @brief 目標 HP 割合を設定する（0.0～1.0）。
             * @param percent 現在 HP / 最大 HP。
             */
            inline void SetHPPercent(float percent) { currentPercent_ = percent; }

        private:
            /**
             * @brief ダメージバーを徐々に現在値へ追従させる。
             */
            void UpdateHPbarAnimation();

            /**
             * @brief 最終フェーズ時の画面揺れ演出。
             */
            void FinalRoundHPbarAnimation();

        private:
            SpriteRender bossIconRender_;        //!< ボス顔アイコン。
            SpriteRender hpBarFrameRender_;      //!< HP バー枠。
            SpriteRender hpBarCurrentBarRender_; //!< 現在 HP（即時反映）。
            SpriteRender hpBarDamageBarRender_;  //!< ダメージ遅延バー。

            float currentPercent_ = 1.0f;     //!< 目標 HP 割合。
            float damagePercent_ = 1.0f;      //!< ダメージバー表示割合。
            float targetPercentSpeed_ = 0.0f; //!< 追従速度（予約）。

            HPbarState hpbarState_ = HPbarState::Normal; //!< 演出モード。
        };
    } // namespace nsUI
} // namespace nsApp

using HPbarState = nsApp::nsUI::HPbarState;
using BossHPbarUI = nsApp::nsUI::BossHPbarUI;

namespace app
{
    namespace nsUI
    {
        using HPbarState = nsApp::nsUI::HPbarState;
        using BossHPbarUI = nsApp::nsUI::BossHPbarUI;
    } // namespace nsUI
} // namespace app
