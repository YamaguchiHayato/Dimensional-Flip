#pragma once
#include "Src/UI/BossUI/BossUIBase.h"
#include <Src/UI/BossUI/BossHPbarUI.h>

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


        private:
            std::vector<BossUIBase*> bossUIParts_;

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

