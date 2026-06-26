#pragma once

namespace nsApp
{
    namespace nsActor
    {
        namespace nsStage
        {
            namespace nsBackGround
            {
                struct FontFormulaEntry
                {
                    Texture texture;
                    Sprite sprite;
                    float worldX = 0.0f;   // ワールドX（横方向のみワールド追従）
                    float screenY = 360.0f; // 画面上Y（中心原点、縦は画面固定）
                    Vector2 scale{1.0f, 1.0f};
                    float parallax = 0.92f;
                    Vector4 mulColor{1.0f, 1.0f, 1.0f, 0.32f};
                };

                struct FontFormulaLayout
                {
                    float worldX = 0.0f;   // ワールドX
                    float screenY = 360.0f; // 画面上Y（中心原点）
                    float scale = 0.185f;
                    float parallax = 0.92f; // 0.9前後=空の奥行き
                    float alpha = 0.32f;
                };

                class FontFormulaBackGround
                {
                public:
                    /**
                     * @brief 初期化。
                     * @param mainRT メインレンダリングターゲット。
                     * @param zprepassTexture ZPrepassで作成された深度テクスチャ。
                     */
                    void Init(RenderTarget& mainRT, Texture& zprepassTexture);

                    /**
                     * @brief フォント数式のテクスチャをパスから読み込む。
                     * @param paths パス。
                     * @param count パスの数。
                     */
                    void LoadFromPaths(const char* const* paths, int count);

                    /**
                     * @brief 更新。
                     * @param cameraWorldX カメラのワールド空間のX座標。 
                     */
                    void Update(float cameraWorldX);

                    /**
                     * @brief 描画。
                     * @param rc レンダリングコンテキスト。
                     * @param mainRT メインレンダリングターゲット。
                     */
                    void Render(RenderContext& rc, RenderTarget& mainRT);

                    /**
                     * @brief 有効化/無効化を設定。
                     * @param enabled flag。
                     */
                    void SetEnabled(bool enabled) { enabled_ = enabled; }


                private:
                    /**
                     * @brief エントリを初期化する。
                     * @param entry 
                     * @param path 
                     * @param screenPos 
                     * @param parallax 
                     */
                    void InitEntry(FontFormulaEntry& entry, const char* path, const FontFormulaLayout& layout);

                    /**
                     * @brief ファイルパスを取得する。
                     * @param name ファイル名。
                     * @return ファイルパス。
                     */
                    std::string GetFilePath(const char*name) const;

                    void EnsureLoaded();

                    Texture* pZprepassTexture_ = nullptr;
                    float cameraWorldX_ = 0.0f;
                    std::vector<std::unique_ptr<FontFormulaEntry>> entries_;
                    Vector2 baseUvOffset_{};
                    bool enabled_ = true;
                    bool isLoaded_ = false;
                    DXGI_FORMAT colorFormat_ = DXGI_FORMAT_UNKNOWN;
                };
            } // namespace nsBackGround
        } // namespace nsStage
    } // namespace nsActor
} // namespace nsApp
