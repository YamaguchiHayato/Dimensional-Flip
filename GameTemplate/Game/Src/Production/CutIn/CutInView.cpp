#include "stdafx.h"

#include "Src/Core/BattlePhaseManager.h"
#include "Src/Core/BossUIManager.h"
#include "Src/Production/CutIn/CutInLogoLayer.h"
#include "Src/Production/CutIn/CutInMaskLayer.h"
#include "Src/Production/CutIn/CutInScrollLayer.h"
#include "Src/Production/CutIn/CutInShadowLayer.h"
#include "Src/Production/CutIn/CutInSlideLayer.h"
#include "Src/Production/CutIn/CutInView.h"

namespace nsApp
{
    namespace nsProduction
    {
        CutInView::~CutInView()
        {
            if (pMaskLayer_)
                DeleteGO(pMaskLayer_);
            if (pScrollLayer_)
                DeleteGO(pScrollLayer_);
            if (pSlideLayer_)
                DeleteGO(pSlideLayer_);
            if (pShadowLayer_)
                DeleteGO(pShadowLayer_);
            if (pLogoLayer_)
                DeleteGO(pLogoLayer_);
        }


        bool CutInView::Start()
        {
            pMaskLayer_ = NewGO<CutInMaskLayer>(0, "MaskLayer");
            pScrollLayer_ = NewGO<CutInScrollLayer>(0, "ScrollLayer");
            pSlideLayer_ = NewGO<CutInSlideLayer>(1, "SlideLayer");
            pShadowLayer_ = NewGO<CutInShadowLayer>(0, "ShadowLayer");
            pLogoLayer_ = NewGO<CutInLogoLayer>(2, "LogoLayer");

            SetLifeDuration(5.0);
            return true;
        }


        void CutInView::Update()
        {
            timer_ += g_gameTime->GetFrameDeltaTime();

            if (IsCutInFinished())
                DeleteGO(this);

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
    } // namespace nsProduction
} // namespace nsApp
