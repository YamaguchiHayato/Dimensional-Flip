#include "stdafx.h"

#include "CompositeBackground.h"

namespace nsK2Engine
{
    void CompositeBackground::Init(RenderTarget& mainRenderTarget, RenderTarget& zprepassRenderTarget)
    {
        /* 初期化。*/
        m_fontFormulaBackGround_.Init(mainRenderTarget, zprepassRenderTarget.GetRenderTargetTexture());
    }


    void CompositeBackground::Update(float cameraWorldX)
    {
        /* 更新。*/
        m_fontFormulaBackGround_.Update(cameraWorldX);
    }

    void CompositeBackground::Render(RenderContext& rc, RenderTarget& mainRenderTarget)
    {
        if (!m_isEnable)
            return;

        /* 描画。*/
        m_fontFormulaBackGround_.Render(rc, mainRenderTarget);
    }
} // namespace nsK2Engine
