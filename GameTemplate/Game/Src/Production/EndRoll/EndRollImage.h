#pragma once
#include "Src/Production/EndRoll/EndRollBase.h"

namespace app
{
    namespace production
    {
        class EndRollImage : public EndRollBase
        {
        public:
            EndRollImage() = default;
            virtual ~EndRollImage() = default;


        public:
            // 初期化処理。
            bool Start() override;

            // 更新処理。
            void Update() override;

            // 描画処理。
            void Render(RenderContext& rc) override;


        private:
            // 画像を初期化する。
            void InitSlideShow();


        private:
            // 画像リスト。
            std::vector<std::unique_ptr<SpriteRender>> images_;

            int currentIndex_ = 0; // 現在表示している画像のインデックス。
            float timer_ = 0.0f;   // 画像を切り替えるタイマー。
        };

    }
}

