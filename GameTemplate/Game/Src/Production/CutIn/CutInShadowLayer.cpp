#include "stdafx.h"
#include "Src/Production/CutIn/CutInShadowLayer.h"
#include "Src/Production/CutIn/CutInSlideLayer.h"
struct layerSize
{
    static constexpr float WIDTH = 225.0f;
    static constexpr float HEIGHT = 185.0f;
};

namespace app
{
    namespace cutIn
    {
        bool CutInShadowLayer::Start()
        {
            const std::string layerName = FindLayerPath("Black");
            layer_.Init(layerName.c_str(), layerSize::WIDTH, layerSize::HEIGHT);

            pTargetLayer_ = nullptr;


            pTargetLayer_ = FindGO<app::cutIn::CutInSlideLayer>("SlideLayer");

            // ターゲットに位置を合わせる。
            // ※一度きり。
            if (offset_.Length() == 0.0)
                offset_ = Vector3(0.0f, 0.0f, 0.0f);

            layer_.SetAlpha(0.0f); // 初期は非表示。
            return true;
        }


        void CutInShadowLayer::Update()
        {
            // 早期リターン。
            if (pTargetLayer_ == nullptr)
                return;

            // ターゲットが振動終了後、なら非表示に。
            if (!pTargetLayer_->IsMoving())
            {
                // ボスの後ろのぴったり張り付いておく(見えないように。)
                currentPos_ = pTargetLayer_->GetCurrentPos();

                // 飛び出す開始地点を更新し続ける。
                ejectStartPos_ = currentPos_;

                // 移動中は隠す。
                layer_.SetAlpha(0.0f);
                ejectTimer_ = 0.0f;
            }

            // 立ち絵の移動が終わったなら。
            else
            {
                // アニメーションが完了するまでの時間。
                if (ejectTimer_ < 1.0f)
                {
                    // 透明度をオンにする。
                    layer_.SetAlpha(0.5f);

                    // タイマーを進行する。
                    float ejectDuration = 0.2f;
                    ejectTimer_ += (1.0f / 60.0f) / ejectDuration;

                    if (ejectTimer_ > 1.0f)
                        ejectTimer_ = 1.0f;

                    // イージング処理。
                    float t = 1.0f - ejectTimer_;
                    float ease = 1.0f - (t * t * t);

                    // ボスの位置からオフセットの位置に移動。
                    Vector3 targetPos = ejectStartPos_ + offset_;

                    // 線形補完。
                    currentPos_ = targetPos + (targetPos - ejectStartPos_) * ease;
                }

                else
                {
                    Vector3 targetPos = pTargetLayer_->GetCurrentPos();
                    currentPos_ = targetPos + offset_;
                }

            }

            // レイヤー画像を更新する。
            layer_.SetPosition(currentPos_);


            float scale = 1.0f;
            layer_.SetScale(Vector3(scale, scale, scale));

            layer_.Update();
        }


        void CutInShadowLayer::Render(RenderContext& rc)
        {
            layer_.Draw(rc);
        }
    }
}
