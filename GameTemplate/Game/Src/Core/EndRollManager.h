#pragma once
#include "Src/Production/EndRoll/EndRollBase.h"
#include <vector>

class Fade;

namespace app
{
    namespace production
    {
        class EndRollImage;
        class EndRollText;

        class EndRollManager : public EndRollBase
        {
        public:
            EndRollManager() = default;
            virtual ~EndRollManager();


        public:
            // 初期化処理。
            bool Start()override;
            // 更新処理。
            void Update() override;
            // 描画処理。
            void Render(RenderContext& rc) override;

            void InitSkipFont();

            void TrySkip();

        // ゲッター。
        public:
            // 終了判定。
            bool IsEnd() const;

        private:
            // エンドロールの構成要素。
            std::vector<EndRollBase*> components_;

            // 画像クラス。
            app::production::EndRollImage* pImage_;

            // テキストクラス。
            app::production::EndRollText* pText_;

            // フェードクラス。
            Fade* pFade_ = nullptr;


        private:
            // フェードアウト開始フラグ。
            bool isFadeOutStarted_ = false;

            // スキップテキスト用。
            FontRender skipFont_;
        };
    }
}
