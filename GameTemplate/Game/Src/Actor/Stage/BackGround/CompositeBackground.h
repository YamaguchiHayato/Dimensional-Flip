#pragma once

#include "FontFormulaBackGround.h"

namespace nsK2Engine
{
    class CompositeBackground : public Noncopyable
    {
    public:
        /**
         * @brief 初期化。
         * @param mainRenderTarget メインレンダリングターゲット。 
         * @param zprepassRenderTarget Zプリパス用レンダリングターゲット。
         */
        void Init(RenderTarget& mainRenderTarget, RenderTarget& zprepassRenderTarget);

        /**
         * @brief 更新。
         * @param cameraWorldX カメラのワールド座標X。 
         */
        void Update(float cameraWorldX);

        /**
         * @brief 描画。
         * @param rc レンダリングコンテキスト。
         * @param mainRenderTarget メインレンダリングターゲット。
         */
        void Render(RenderContext& rc, RenderTarget& mainRenderTarget);

        /**
         * @brief 有効化。
         */
        inline void Enable()
        {
            m_isEnable = true;
        }

        /**
         * @brief 無効化。
         */
        inline void Disable()
        {
            m_isEnable = false;
        }


    private:
        nsApp::nsActor::nsStage::nsBackGround::FontFormulaBackGround m_fontFormulaBackGround_;
        bool m_isEnable = false;
    };
} // namespace nsK2Engine
