#include "stdafx.h"
#include "Src/production/CutIn/CutInView.h"
#include "Src/production/CutIn/CutInLogoLayer.h"
#include "Src/production/CutIn/CutInMaskLayer.h"
#include "Src/production/CutIn/CutInScrollLayer.h"
#include "Src/production/CutIn/CutInShadowLayer.h"
#include "Src/production/CutIn/CutInSlideLayer.h"


namespace app
{
    namespace cutIn
    {
        CutInView::~CutInView()
        {
            // 背景レイヤーの破棄。
            if (pMaskLayer_)
                DeleteGO(pMaskLayer_);

            // スクロールレイヤーの破棄。
            if (pScrollLayer_)
                DeleteGO(pScrollLayer_);

            // キャラレイヤーの廃棄。
            if (pSlideLayer_)
                DeleteGO(pSlideLayer_);

            // 影レイヤーの破棄。
            if (pShadowLayer_)
                DeleteGO(pShadowLayer_);

            // ロゴレイヤーの破棄。
            if (pLogoLayer_)
                DeleteGO(pLogoLayer_);
        }


        bool CutInView::Start()
        {
            // 各レイヤーの初期化。
            // カットインの黒背景。
            pMaskLayer_ = NewGO<app::cutIn::CutInMaskLayer>(0, "MaskLayer");
            // キャラ背景の雷画像のレイヤー。
            pScrollLayer_ = NewGO<app::cutIn::CutInScrollLayer>(0, "ScrollLayer");
            // ボスのカットイン画像。
            pSlideLayer_ = NewGO<app::cutIn::CutInSlideLayer>(1, "SlideLayer");
            // ボスの影レイヤー画像。
            pShadowLayer_ = NewGO<app::cutIn::CutInShadowLayer>(0, "ShadowLayer");
            // ボスのエンブレムレイヤー。
            pLogoLayer_ = NewGO<app::cutIn::CutInLogoLayer>(2, "LogoLayer");
            return true;
        }


        void CutInView::Update()
        {
            // 時間制限が設定されている場合、カウントアップ。
            if (lifeDuration_ > 0.0f)
            {
                timer_ += 1.0f / 60.0f;

                if (timer_ >= lifeDuration_)
                {
                    timer_ = lifeDuration_;
                    return;
                }
            }

            layer_.Update();
        }
    }
}
