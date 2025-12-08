#include "stdafx.h"
#include "CutInSlideLayer.h"

struct layerSize
{
    static constexpr auto WIDHT = 220.0;  // カットイン黒背景の横幅。   static constexpr auto WIDHT = 220.0;  // カットイン黒背景の横幅。
    static constexpr auto HEIGHT = 185.0f; // カットイン黒背景の縦幅。
};

namespace app
{
    namespace cutIn
    {
        bool CutInSlideLayer::Start()
        {
//            const std::string layerPath_Normal = FindLayerPath("StrongEnemy");
            const std::string layerPath_Normal = FindLayerPath("BOSS");
            layer_.Init(layerPath_Normal.c_str(), layerSize::WIDHT, layerSize::HEIGHT);

            // 開始位置の初期化。
            InitStartPos();
            // スライド中のパラメータの初期化。
            // ここでは、 移動所要時間、移動時間、移動フラグを設定する。
            InitSlideParam();

            // タイマーをセットする。
            InitWaitTimer();
            return true;    
        }


        void CutInSlideLayer::Update()
        {

            // 待ち時間が終わるまで処理をしない。
            if (waitTimer_ < waitTime_)
            {
                waitTimer_ += 1.0f / 60.0f;

                // 待機中は画面外に置いておく。
                layer_.SetPosition(startPos_);
                layer_.Update();
                return;
            }
            //////////////////////////////////////
            // 待機時間が終わると以下が処理される。
            //////////////////////////////////////

            // スライド処理。
            Slide();

            if (!isMoving_)
            {

            }

            else
            {
                float scale = 3.0f;
                layer_.SetScale(Vector3(scale, scale, scale));
            }

            layer_.SetPosition(currentPos_);
            layer_.Update();

        }


        void CutInSlideLayer::Slide()
        {
            if (isMoving_)
            {
                // タイマーを進める。
                float deltaTime = 1.0f / 60.0f;
                timer_ += deltaTime;

                // 移動歩率を計算する。
                float rate = timer_ / duration_;

                if (rate >= 1.0f)
                {
                    rate = 1.0f;
                    // 移動完了。
                    isMoving_ = false;
                }

                // イージング処理。
                float t = 1.0f - rate;
                float easeRate = 1.0f - (t * t * t);

                // 線形補完を使って、現在位置を決定する。
                currentPos_ = startPos_ + (endPos_ - startPos_) * easeRate;
            }
        }


        void CutInSlideLayer::Breathing()
        {
            // タイマーを進行する。
            breathTimer_ += 1.0f / 60.0f;

            // sin波を使い、動かす。
            float scale = 1.0f + sinf(breathTimer_ * 5.0f) * 0.05f;

            layer_.SetScale(Vector3(scale, scale, scale));
        }


        void CutInSlideLayer::Render(RenderContext& rc)
        {
            layer_.Draw(rc);
        }
    }
}
