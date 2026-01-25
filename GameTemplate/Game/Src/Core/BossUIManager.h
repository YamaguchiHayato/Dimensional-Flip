#pragma once
#include "Src/UI/BossUI/BossUIBase.h"

#include "Src/UI/BossUI/BossAttackIndicatorUI.h"
#include "Src/UI/BossUI/BossHPbarUI.h"

namespace app
{
    namespace nsUI
    {
        class BossHPbarUI;

        class BossUIManager : public BossUIBase
        {
        private:
            BossUIManager() = default;
            virtual ~BossUIManager() = default;


        private:
            // 管理するUIクラスを宣言。
            void MakeHPbarUI();

            // 攻撃予告UIの生成。
            void MakeAttackIndicatorUI();


        public:
            // 攻撃状態の切り替え通知を受け取る関数。
            inline void OnNotifyAttack(BossAttackKind kind)
            {
                if (pBossAttackIndicatorUI_)
                    pBossAttackIndicatorUI_->SetAttackKind(kind);
            }


        private:
            std::vector<BossUIBase*> bossUIParts_;
            BossAttackIndicatorUI* pBossAttackIndicatorUI_ = nullptr;
            
            // 各UI。
            BossHPbarUI* pBossHPbarUI_ = nullptr; // HPバーUI。




        public:
            // コピームーブを禁止する。
            BossUIManager(const BossUIManager&) = delete;
            void operator=(const BossUIManager&) = delete;


        public:
            // アクセサ。
            inline static BossUIManager& GetInstance()
            {
                static BossUIManager instance;
                return instance;
            }


            // このクラスのサイクル処理。
            // 初期化。
            void Initialize();

            // 更新。
            void Update();

            // 描画。
            void Draw(RenderContext& rc) override;

        public:
            // HPの更新。
            void OnUpdateHP(float currentHP, float maxHP);

            // Phase, 攻撃状態の切り替え。
            void OnChangePhase(uint8_t stateID);


        };

    }
}

