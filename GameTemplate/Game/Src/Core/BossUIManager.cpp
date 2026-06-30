#include "stdafx.h"

#include "BossUIManager.h"
#include "Src/UI/BossUI/BossHPbarUI.h"

namespace nsApp
{
    namespace nsUI
    {
        void BossUIManager::Initialize()
        {
            /**
             * @brief 各 UI パーツを生成
             */
            MakeHPbarUI();
            MakeAttackIndicatorUI();

            /**
             * @brief ボス HUD の基準位置（画面右下付近）
             */
            transform_.localPosition_ = Vector3(650.0f, -400.0f, 0.0f);
        }

        void BossUIManager::Update()
        {
            /**
             * @brief 親の行列を更新
             */
            UpdateMatrix();

            /**
             * @brief 子 UI パーツを更新
             */
            for (auto* pPart : bossUIParts_)
            {
                pPart->Update();
                pPart->UpdateMatrix();
            }
        }

        void BossUIManager::Draw(RenderContext& rc)
        {
            /**
             * @brief 子 UI パーツを描画
             */
            for (auto* pPart : bossUIParts_)
                pPart->Draw(rc);
        }

        void BossUIManager::OnUpdateHP(float currentHP, float maxHP)
        {
            /**
             * @brief HP 割合を HP バーに渡す
             */
            const float percent = currentHP / maxHP;

            if (pBossHPbarUI_)
                pBossHPbarUI_->SetHPPercent(percent);
        }

        void BossUIManager::MakeHPbarUI()
        {
            /**
             * @brief HP バーを生成し、自身を親として登録
             */
            auto* hpBar = new app::nsUI::BossHPbarUI();
            hpBar->SetParent(this);
            hpBar->Initialize();

            pBossHPbarUI_ = hpBar;
            bossUIParts_.push_back(hpBar);
        }

        void BossUIManager::MakeAttackIndicatorUI()
        {
            /**
             * @brief 攻撃予告アイコン UI を生成
             */
            pBossAttackIndicatorUI_ = new app::nsUI::BossAttackIndicatorUI();
            pBossAttackIndicatorUI_->Initialize();
            pBossAttackIndicatorUI_->SetParent(this);

            bossUIParts_.push_back(pBossAttackIndicatorUI_);
        }
    } // namespace nsUI
} // namespace nsApp
