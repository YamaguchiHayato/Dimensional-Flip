#pragma once
#include "Src/Direction/CutIn/CutInBase.h"

namespace app
{
    namespace cutIn
    {
        class CutInLogoLayer : public CutInBase
        {
        public:
            CutInLogoLayer() = default;
            virtual ~CutInLogoLayer() = default;

            // 初期化処理。
            bool Start() override;
            // 更新処理。
            void Update() override;
            // 描画処理。
            void Render(RenderContext& rc) override;
            // レイヤーのパスを取得する。
            // 再起関数。
            inline const std::string FindLayerPath(const std::string layerName) const override
            {
                return CutInBase::FindLayerPath(layerName);
            }

        private:
            // 回転処理。
            void Move();

            // ステータスの初期化。
            void Preparation();

            // 描画を承認する。
            inline void CanDrawEmblem()
            {
                isVisible = true;
                isAnimating_ = true; // 回転開始。
                moveTimer_ = 0.0f;   // タイマーリセット。
            }

            // 完了判定。
            inline bool IsFinished() const
            {
                return isVisible && !isAnimating_;
            }

        private:
            // 待機用。
            float delayTimer_ = 0.0f; // 待機時間計測タイマー
            float delayTime_ = 0.5f;  // 待ち時間。
            bool isVisible = false;   // 表示フラグ。

            // 回転用。
            float moveTimer_ = 0.0f;  // 計測タイマー。
            float duration_ = 0.6f;   // 所要時間。
            bool isAnimating_ = false;// 回転中フラグ。

            Quaternion currentRot_ = Quaternion::Identity; // 現在の回転角。
        };

    }
}

