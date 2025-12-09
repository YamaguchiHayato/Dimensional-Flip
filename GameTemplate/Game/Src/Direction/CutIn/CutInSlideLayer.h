#pragma once
#include "Src/Direction/CutIn/CutInBase.h"

namespace app
{
    namespace cutIn
    {
        class CutInSlideLayer : public CutInBase
        {
        public:
            CutInSlideLayer() = default;
            virtual ~CutInSlideLayer() = default;

            // 初期化処理。
            bool Start() override;
            // 更新処理。
            void Update() override;
            // 描画処理。
            void Render(RenderContext& rc) override;
            // レイヤーのパスを取得する。
            inline const std::string FindLayerPath(const std::string layerName) const override
            {
                return CutInBase::FindLayerPath(layerName);
            }

        private:
            // スライド処理。
            void Slide();


            // 拡大・縮小処理。
            void Breathing();


            // 初期地点を設定する。
            inline void InitStartPos()
            {
                // 右から左へスライドイン。
                startPos_ = Vector3(500.0f, 0.0f, 0.0f);
                endPos_ = Vector3(-300.0f, 0.0f, 0.0f);
                currentPos_ = startPos_;
            }


            // パラメータ設定。
            inline void InitSlideParam()
            {
                duration_ = 0.5f; // 所要時間。
                timer_ = 0.0f;
                isMoving_ = true;
            }


            // タイマー設定。
            inline void InitWaitTimer()
            {
                waitTimer_ = 0.0f;
                waitTime_ = 0.8f;
            }

        // ゲッター。
        public:
            // 現在座標の取得。
            inline const Vector3& GetCurrentPos() const
            {
                return currentPos_;
            }

            // 移動中かどうか。
            inline bool IsMoving() const
            {
                return isMoving_;
            }

        private:
            Vector3 startPos_ = Vector3::Zero;        // 開始位置。
            Vector3 endPos_ = Vector3::Zero;          // 終了位置。
            Vector3 currentPos_ = Vector3::Zero;      // 現在位置。


            float timer_ = 0.0f;                       // タイマー。
            float duration_ = 1.0f;                    // 所要時間。
            float breathTimer_ = 0.0f;                 // 呼吸タイマー。

            bool isMoving_;

            // 開始待ちタイマー。
            float waitTimer_ = 0.0f;                   // 待機タイマー。
            float waitTime_ = 0.6f;// ロゴの演出時間。
        };

    }
}

