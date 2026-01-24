#pragma once
#include "Src/UI/BossUI/BossUIBase.h"

namespace app
{
    namespace nsUI
    {
        // 演出の状態を管理する列挙型。
        enum class HPbarState
        {
            Normal,
            FinalPhase,
        };


        class BossHPbarUI : public BossUIBase
        {
        public:
            BossHPbarUI() = default;
            virtual ~BossHPbarUI() = default;


        public:
            // 4枚の画像を初期化。
            void Initialize() ;

            // HPbarの減少を計算。
            void Update()override;

            // 描画処理。
            void Draw(RenderContext& rc) override;


        // セッター。
        public:
            // HPbarの割合を計算。
            inline void SetHPPercent(float percent)
            {
                currentPercent_ = percent;
            }


        // ヘルパー。
        private:
            // HPbarの割合をアニメーションで更新。
            void UpdateHPbarAnimation();

            // FinalRound時にのみ演出。
            void FinalRoundHPbarAnimation();

        private:
            SpriteRender bossIconRender_;
            SpriteRender hpBarFrameRender_;
            SpriteRender hpBarCurrentBarRender_;
            SpriteRender hpBarDamageBarRender_;


        // HPバーの割合用の変数群。
        private:
            float currentPercent_ = 1.0f;
            float damagePercent_ = 1.0f;
            float targetPercentSpeed_ = 0.0f;


        private:
            HPbarState hpbarState_ = HPbarState::Normal;
        };

    }
}
