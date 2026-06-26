#include "stdafx.h"

#include "FontFormulaBackGround.h"

namespace
{
    using namespace nsApp::nsActor::nsStage::nsBackGround;

    static const char* kFormulaNames[] = {
        "pattern1",  "pattern2",  "pattern3",  "pattern4",  "pattern5",
        "pattern6",  "pattern7",  "pattern8",  "pattern9",  "pattern10",
        "pattern11", "pattern12", "pattern13", "pattern14", "pattern15",
        "pattern16", "pattern17", "pattern18", "pattern19", "pattern20",
        "pattern21", "pattern22", "pattern23", "pattern24", "pattern25",
    };

    // worldX間隔275（従来の約半分）→ 同時表示8〜11個程度
    static const FontFormulaLayout kFormulaLayouts[] = {
        { -3900.f, 250.f, 0.188f, 0.88f, 0.34f },
        { -3625.f,  70.f, 0.176f, 0.91f, 0.33f },
        { -3350.f, -55.f, 0.198f, 0.89f, 0.32f },
        { -3075.f, 220.f, 0.182f, 0.93f, 0.34f },
        { -2800.f,  35.f, 0.191f, 0.90f, 0.33f },
        { -2525.f, 275.f, 0.174f, 0.92f, 0.31f },
        { -2250.f,  95.f, 0.186f, 0.88f, 0.34f },
        { -1975.f, -40.f, 0.180f, 0.94f, 0.32f },
        { -1700.f, 245.f, 0.195f, 0.89f, 0.35f },
        { -1425.f,  60.f, 0.178f, 0.91f, 0.33f },
        { -1150.f, 185.f, 0.184f, 0.90f, 0.32f },
        {  -875.f, -70.f, 0.172f, 0.93f, 0.31f },
        {  -600.f, 125.f, 0.190f, 0.88f, 0.34f },
        {  -325.f, 265.f, 0.177f, 0.92f, 0.31f },
        {   -50.f,  15.f, 0.193f, 0.89f, 0.33f },
        {   225.f, 200.f, 0.181f, 0.91f, 0.34f },
        {   500.f, -25.f, 0.187f, 0.90f, 0.32f },
        {   775.f,  85.f, 0.175f, 0.94f, 0.33f },
        {  1050.f, 255.f, 0.189f, 0.88f, 0.35f },
        {  1325.f,  45.f, 0.183f, 0.92f, 0.32f },
        {  1600.f, 165.f, 0.176f, 0.89f, 0.34f },
        {  1875.f, -85.f, 0.179f, 0.91f, 0.31f },
        {  2150.f, 105.f, 0.185f, 0.90f, 0.33f },
        {  2425.f,  50.f, 0.173f, 0.93f, 0.32f },
        {  2700.f, 230.f, 0.172f, 0.88f, 0.34f },
    };

    static_assert(_countof(kFormulaNames) == _countof(kFormulaLayouts), "kFormulaNames and kFormulaLayouts must match");

    const FontFormulaLayout& GetLayout(int index)
    {
        if (index >= 0 && index < static_cast<int>(_countof(kFormulaLayouts)))
            return kFormulaLayouts[index];

        static const FontFormulaLayout kFallback = {0.f, 120.f, 0.185f, 0.91f, 0.32f};
        return kFallback;
    }
}

namespace nsApp
{
    namespace nsActor
    {
        namespace nsStage
        {
            namespace nsBackGround
            {
                void FontFormulaBackGround::Init(RenderTarget& mainRT, Texture& zprepassTexture)
                {
                    pZprepassTexture_ = &zprepassTexture;
                    colorFormat_ = mainRT.GetColorBufferFormat();
                    entries_.clear();
                    isLoaded_ = false;
                    cameraWorldX_ = 0.0f;
                }

                void FontFormulaBackGround::EnsureLoaded()
                {
                    if (isLoaded_ || pZprepassTexture_ == nullptr)
                        return;

                    LoadFromPaths(kFormulaNames, static_cast<int>(_countof(kFormulaNames)));

                    if (!entries_.empty())
                        isLoaded_ = true;
                }

                void FontFormulaBackGround::LoadFromPaths(const char* const* paths, int count)
                {
                    if (pZprepassTexture_ == nullptr || paths == nullptr || count <= 0)
                        return;

                    entries_.clear();
                    entries_.reserve(static_cast<size_t>(count));

                    for (int i = 0; i < count; ++i)
                    {
                        if (paths[i] == nullptr || paths[i][0] == '\0')
                            continue;

                        auto entry = std::make_unique<FontFormulaEntry>();
                        InitEntry(*entry, paths[i], GetLayout(i));

                        if (entry->texture.IsValid())
                            entries_.push_back(std::move(entry));
                    }
                }

                void FontFormulaBackGround::InitEntry(FontFormulaEntry& entry, const char* path,
                                                      const FontFormulaLayout& layout)
                {
                    const std::string filePath = GetFilePath(path);

                    wchar_t wPath[512] = {};
                    mbstowcs(wPath, filePath.c_str(), _countof(wPath) - 1);

                    entry.texture.InitFromDDSFile(wPath);

                    if (!entry.texture.IsValid())
                    {
                        std::string pngPath = std::string("Assets/stage/BackGround/FontBackGround/") + path + ".png";
                        wchar_t wPngPath[512] = {};
                        mbstowcs(wPngPath, pngPath.c_str(), _countof(wPngPath) - 1);
                        entry.texture.InitFromWICFile(wPngPath);
                    }

                    const UINT texW = static_cast<UINT>(entry.texture.GetWidth());
                    const UINT texH = static_cast<UINT>(entry.texture.GetHeight());
                    if (texW == 0 || texH == 0)
                        return;

                    SpriteInitData initData = {};
                    initData.m_textures[0] = &entry.texture;
                    initData.m_textures[1] = pZprepassTexture_;
                    initData.m_width = texW;
                    initData.m_height = texH;
                    initData.m_fxFilePath = "Assets/shader/CompositeBackground.fx";
                    initData.m_alphaBlendMode = AlphaBlendMode_Trans;
                    initData.m_colorBufferFormat[0] = colorFormat_;

                    entry.sprite.Init(initData);
                    entry.worldX = layout.worldX;
                    entry.screenY = layout.screenY;
                    entry.scale = {layout.scale, layout.scale};
                    entry.parallax = layout.parallax;
                    entry.mulColor = {1.0f, 1.0f, 1.0f, layout.alpha};
                }

                void FontFormulaBackGround::Update(float cameraWorldX)
                {
                    cameraWorldX_ = cameraWorldX;
                    EnsureLoaded();
                }

                void FontFormulaBackGround::Render(RenderContext& rc, RenderTarget& mainRT)
                {
                    if (!enabled_)
                        return;

                    EnsureLoaded();

                    if (entries_.empty())
                        return;

                    rc.WaitUntilToPossibleSetRenderTarget(mainRT);
                    rc.SetRenderTargetAndViewport(mainRT);

                    const float halfScreenW = static_cast<float>(FRAME_BUFFER_W) * 0.5f;
                    constexpr float kCullMargin = 450.0f;
                    const float cullMin = -halfScreenW - kCullMargin;
                    const float cullMax = halfScreenW + kCullMargin;

                    for (auto& entry : entries_)
                    {
                        if (!entry->texture.IsValid())
                            continue;

                        // ワールド配置 + パララックス（空はカメラより遅く流れる）
                        Vector3 pos = {
                            entry->worldX - cameraWorldX_ * entry->parallax + halfScreenW,
                            entry->screenY,
                            0.0f
                        };

                        if (pos.x < cullMin || pos.x > cullMax)
                            continue;

                        const float halfScreenH = static_cast<float>(FRAME_BUFFER_H) * 0.5f;
                        const float halfSpriteH = static_cast<float>(entry->texture.GetHeight())
                            * entry->scale.y * 0.5f;
                        constexpr float kTopMargin = 20.0f;
                        const float maxCenterY = halfScreenH - halfSpriteH - kTopMargin;
                        if (pos.y > maxCenterY)
                            pos.y = maxCenterY;

                        const Vector3 scale = {entry->scale.x, entry->scale.y, 1.0f};

                        entry->sprite.SetMulColor(entry->mulColor);
                        entry->sprite.Update(pos, Quaternion::Identity, scale, Vector2{0.0f, 0.0f});
                        entry->sprite.Draw(rc);
                    }

                    rc.WaitUntilFinishDrawingToRenderTarget(mainRT);
                }

                std::string FontFormulaBackGround::GetFilePath(const char* name) const
                {
                    return std::string("Assets/stage/BackGround/FontBackGround/") + name + ".DDS";
                }
            } // namespace nsBackGround
        } // namespace nsStage
    } // namespace nsActor
} // namespace nsApp
