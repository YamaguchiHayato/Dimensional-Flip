#pragma once

namespace nsK2Engine
{
    /**
     * @brief CompositeBackgroundクラス。
     * @details このクラスは、背景の合成描画を管理するためのクラスです。背景テクスチャを使用して、レンダリングターゲットに対して背景を描画します。
     */
    class CompositeBackground : public Noncopyable
    {
    public:
        /**
         * @brief CompositeBackgroundクラスの初期化。
         * @param mainRenderTarget 
         * @param zprepassRenderTarget  
         * @param bgTexturePath 
         */
        void Init(RenderTarget& mainRenderTarget, RenderTarget& zprepassRenderTarget,
                  const char* bgTexturePath =
                      "Assets/sprite/fontBackGround.DDS");

        /**
         * @brief CompositeBackgroundクラスのレンダリング。
         * @param rc 
         * @param mainRenderTarget 
         */
        void Render(RenderContext& rc, RenderTarget& mainRenderTarget);

        /**
         * @brief CompositeBackgroundクラスの有効化。
         */
        inline void Enable() { m_isEnable = true; }

        /**
         * @brief CompositeBackgroundクラスの無効化。
         */
        inline void Disable() { m_isEnable = false; }

        /**
         * @brief CompositeBackgroundクラスのUVオフセットを設定。
         * @param offset 
         */
        void SetUvOffset(const Vector2& offset) { m_uvOffset = offset; }

        /**
         * @brief CompositeBackgroundクラスの乗算カラーを設定。
         * @param color 
         */
        void SetMulColor(const Vector4& color) { m_mulColor = color; }


    private:
        /**
         * @brief CompositeBackgroundクラスのスプライトを初期化。
         * @param 
         */
        void InitSprite(...);

        Sprite m_sprite;     // CompositeBackground.fx 用
        Texture m_bgTexture; // fontBackGround.DDS
        Vector2 m_uvOffset{};
        Vector4 m_mulColor{1, 1, 1, 1};
        bool m_isEnable = false; //! デフォルトは描画オフ。
    };

} // namespace nsK2Engine
