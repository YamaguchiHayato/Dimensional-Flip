#pragma once

#include "Src/UI/BossUI/BossAttackIndicatorUI.h"
#include "Src/UI/BossUI/BossHPbarUI.h"
#include "Src/UI/BossUI/BossPhaseUI.h"
#include "Src/UI/BossUI/BossUIBase.h"

/**
 * @file   BossUIManager.h
 * @brief  ボス戦 HUD（HP・攻撃予告・フェーズ演出）を統括するシングルトン。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class BossUIManager
         * @brief ボス戦 UI パーツの生成・更新・描画をまとめて行う。
         * @note  各パーツ（BossHPbarUI 等）は app::nsUI に残る。段階的移行用。
         */
        class BossUIManager : public BossUIBase
        {
        public:
            /**
             * @brief シングルトンインスタンスを返す。
             * @return BossUIManager の唯一のインスタンス。
             */
            inline static BossUIManager& GetInstance()
            {
                static BossUIManager instance;
                return instance;
            }

            BossUIManager(const BossUIManager&) = delete;
            void operator=(const BossUIManager&) = delete;

            /**
             * @brief HP バー・攻撃予告 UI を生成し、初期位置を設定する。
             */
            void Initialize();

            /**
             * @brief 各 UI パーツの行列更新と Update を呼ぶ。
             */
            void Update();

            /**
             * @brief 各 UI パーツを描画する。
             * @param rc レンダリングコンテキスト。
             */
            void Draw(RenderContext& rc) override;

            /**
             * @brief ボス HP の変化を HP バー UI に反映する。
             * @param currentHP 現在 HP。
             * @param maxHP     最大 HP。
             */
            void OnUpdateHP(float currentHP, float maxHP);

            /**
             * @brief 攻撃予告アイコンを切り替える。
             * @param kind 表示する攻撃種別。
             */
            inline void OnNotifyAttack(app::nsUI::BossAttackKind kind)
            {
                if (pBossAttackIndicatorUI_)
                    pBossAttackIndicatorUI_->SetAttackKind(kind);
            }

            /**
             * @brief フェーズ演出が再生中かどうかを返す。
             * @return 再生中なら true。
             */
            inline bool IsPhasePlaying() const { return pBossPhaseUI_ && pBossPhaseUI_->IsPhasePlaying(); }

            /**
             * @brief フェーズ演出中にアクター更新を止めるべきかを返す。
             * @return 止めるべきなら true。
             */
            inline bool ShouldStopActors() const { return pBossPhaseUI_ && pBossPhaseUI_->IsPhasePlaying(); }

            /**
             * @brief フェーズ切り替え演出を開始する。
             * @param stateID BossPhaseKind にキャストされるフェーズ ID。
             */
            inline void OnChangePhase(uint8_t stateID)
            {
                if (pBossPhaseUI_)
                    pBossPhaseUI_->StartPhaseAninm(static_cast<app::nsUI::BossPhaseKind>(stateID));
            }


            void Shutdown()
            {
                for (auto* p : bossUIParts_)
                    delete p;

                bossUIParts_.clear();
                pBossHPbarUI_ = nullptr;
                pBossAttackIndicatorUI_ = nullptr;
            }


        private:
            /* コンストラクタとデストラクタ。*/
            BossUIManager() = default;
            virtual ~BossUIManager() = default;

            /**
             * @brief HP バー UI を生成して管理リストに登録する。
             */
            void MakeHPbarUI();

            /**
             * @brief 攻撃予告 UI を生成して管理リストに登録する。
             */
            void MakeAttackIndicatorUI();

            /**
             * @brief フェーズ演出 UI を生成する（予約）。
             */
            void MakePhaseUI();

        private:
            std::vector<app::nsUI::BossUIBase*> bossUIParts_;                    //!< 管理下の UI パーツ一覧。
            BossAttackIndicatorUI* pBossAttackIndicatorUI_ = nullptr; //!< 攻撃予告 UI。
            BossHPbarUI* pBossHPbarUI_ = nullptr;                     //!< HP バー UI。
            BossPhaseUI* pBossPhaseUI_ = nullptr;                     //!< フェーズ演出 UI。
        };
    } // namespace nsUI
} // namespace nsApp

using BossUIManager = nsApp::nsUI::BossUIManager;

namespace app
{
    namespace nsUI
    {
        using BossUIManager = nsApp::nsUI::BossUIManager;
    }
} // namespace app
