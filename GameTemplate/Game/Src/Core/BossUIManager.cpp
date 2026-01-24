#include "stdafx.h"
#include "BossUIManager.h"

// 各UIクラス。
#include "Src/UI/BossUI/BossHPbarUI.h"


namespace app
{
    namespace nsUI
    {
        void BossUIManager::Initialize()
        {
            // 各UIクラスを生成する。
            // BPbarUIの生成。
            MakeHPbarUI();


            // 初期位置を設定。
            transform_.localPosition_ = Vector3(650, -400.0f, 0.0f);
        }


        void BossUIManager::Update()
        {
            // 行列更新を行う。
            UpdateMatrix();

            // 各UIクラスの更新を行う。
            for (auto* bossUIParts : bossUIParts_)
            {
                bossUIParts->Update();
                bossUIParts->UpdateMatrix();
            }
        }


        void BossUIManager::Draw(RenderContext& rc)
        {
            // 各UIクラスの描画を行う。
            for (auto* bossUIParts : bossUIParts_)
            {
                bossUIParts->Draw(rc);
            }
        }


        void BossUIManager::OnChangePhase(uint8_t stateID)
        {

        }


        void BossUIManager::OnUpdateHP(float currentHP, float maxHP)
        {
            auto percent = currentHP / maxHP;

            // HPバーUIに通知。
            if (pBossHPbarUI_)
                pBossHPbarUI_->SetHPPercent(percent);
        }


        void BossUIManager::MakeHPbarUI()
        {
            // HPバーUIの生成。
            auto hpBar = new BossHPbarUI();

            // 管理配列に追加。
            // 自信を親として設定。
            hpBar->SetParent(this);

            hpBar->Initialize();

            // メンバ変数に設定。
            pBossHPbarUI_ = hpBar;
            // リストに追加。
            bossUIParts_.push_back(hpBar);
        }
    }
}
