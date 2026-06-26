#pragma once

#include "Src/Actor/Stage/BackGround/FontFormulaBackGround.h"

namespace nsK2Engine
{
    class CompositeBackground : public Noncopyable
    {
    public:
        void Init(RenderTarget& mainRenderTarget, RenderTarget& zprepassRenderTarget);
        void Update(float cameraWorldX);
        void Render(RenderContext& rc, RenderTarget& mainRenderTarget);

        inline void Enable() { m_isEnable = true; }
        inline void Disable() { m_isEnable = false; }


    private:
        nsApp::nsActor::nsStage::nsBackGround::FontFormulaBackGround m_fontFormulaBackGround_;
        bool m_isEnable = false;
    };
} // namespace nsK2Engine
