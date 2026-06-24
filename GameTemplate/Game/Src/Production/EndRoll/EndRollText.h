#pragma once
#include "Src/Production/EndRoll/EndRollBase.h"

namespace app
{
    namespace production
    {
        class EndRollText : public EndRollBase
        {
        public:
            EndRollText() = default;
            virtual ~EndRollText() = default;


        public:
            // 初期化処理。
            bool Start() override;

            // 更新処理。
            void Update() override;

            // 描画処理。
            void Render(RenderContext& rc) override;

            // スキップ処理（スクロールを即終了）。
            void Skip()
            {
                isEnd_ = true;
            }


        private:
            // 1行のTextを管理する構造体。
            struct StaffRollLine
            {
                FontRender staffRollText_;              // スタッフロールを描画。
                Vector3 initializePos_ = Vector3::Zero; // スタッフロールの初期位置。
            };

            std::vector<std::unique_ptr<StaffRollLine>> lines_;          // スタッフロールの行の集合。

            float scrollSpeed_ = 1.0f;                  // スクロール速度。


        private:
            // 表示する文字を管理する。
            bool ManageText();
        };
    }
}
