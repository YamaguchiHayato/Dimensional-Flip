#include "stdafx.h"

#include "CompositeBackground.h"

namespace nsK2Engine
{

    void CompositeBackground::Init(RenderTarget& mainRenderTarget, RenderTarget& zprepassRenderTarget,
                                   const char* bgTexturePath)
    {
        wchar_t wPath[512] = {};
        mbstowcs(wPath, bgTexturePath, _countof(wPath) - 1);
        m_bgTexture.InitFromDDSFile(wPath);

        SpriteInitData initData = {};
        initData.m_textures[0] = &m_bgTexture;
        initData.m_textures[1] = &zprepassRenderTarget.GetRenderTargetTexture();
        initData.m_width = mainRenderTarget.GetWidth();
        initData.m_height = mainRenderTarget.GetHeight();
        initData.m_fxFilePath = "Assets/shader/CompositeBackground.fx";
        initData.m_alphaBlendMode = AlphaBlendMode_Trans;
        initData.m_colorBufferFormat[0] = mainRenderTarget.GetColorBufferFormat();

        m_sprite.Init(initData);
        m_sprite.SetMulColor(m_mulColor);
        m_sprite.Update(Vector3::Zero, Quaternion::Identity, Vector3::One, Sprite::DEFAULT_PIVOT);
    }

    void CompositeBackground::Render(RenderContext& rc, RenderTarget& mainRenderTarget)
    {
        if (!m_isEnable)
            return;

        rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
        rc.SetRenderTargetAndViewport(mainRenderTarget);

        m_sprite.SetMulColor(m_mulColor);
        m_sprite.Update(Vector3::Zero, Quaternion::Identity, Vector3::One, Sprite::DEFAULT_PIVOT);
        m_sprite.Draw(rc);

        rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
    }

} // namespace nsK2Engine
