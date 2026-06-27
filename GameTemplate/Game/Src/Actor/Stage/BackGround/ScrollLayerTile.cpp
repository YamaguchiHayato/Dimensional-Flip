#include "stdafx.h"

#include "Src/Actor/Stage/BackGround/ScrollLayerTile.h"

namespace
{
    using nsApp::nsStage::nsScrollBackGround::ScrollLayerDefinition;

    constexpr const char* kScrollBackGroundFxPath = "Assets/shader/ScrollBackGround.fx";
    constexpr float kDefaultCullMargin = 400.0f;

    struct LayerDisplayTransform
    {
        float screenX = 0.0f;
        float screenY = 0.0f;
        float scaleX = 1.0f;
        float scaleY = 1.0f;
        float tileWidth = 0.0f;
    };


    float CalcScreenScrollX(float cameraWorldX, int tileIndex, int tileCount, float tileWidth, float parallax, float parallaxPixelScale)
    {
        /* 画面を基点としたスクロールを計算する。*/
        const float halfScreenW = static_cast<float>(FRAME_BUFFER_W) * 0.5f;
        const int safeTileCount = tileCount > 0 ? tileCount : 1;
        const float loopLength = tileWidth * static_cast<float>(safeTileCount);
        float scrollPx = cameraWorldX * parallax * parallaxPixelScale;
        float phase = std::fmod(scrollPx, loopLength);

        /* fmod の結果が負の値になる場合があるので、正の値に補正する。*/
        if (phase < 0.0f)
            phase += loopLength;

        /* タイルの中心位置を計算する。*/
        return -phase + static_cast<float>(tileIndex) * tileWidth + tileWidth * 0.5f - halfScreenW;
    }


    LayerDisplayTransform CalcLayerDisplayTransform(const ScrollLayerDefinition& definition, int tileIndex, float cameraWorldX, UINT textureWidth, UINT textureHeight)
    {
        /* レイヤーの表示位置とスケールを計算する。*/
        LayerDisplayTransform out = {};

        /* 画面サイズを取得する。*/
        const float screenW = static_cast<float>(FRAME_BUFFER_W);
        const float screenH = static_cast<float>(FRAME_BUFFER_H);
        const float halfScreenH = screenH * 0.5f;

        /* タイル幅を設定する。*/
        out.tileWidth = screenW;
        const int tileCount = definition.tileCount > 0 ? definition.tileCount : 1;

        /* スクロール位置を計算する。*/
        out.screenX = CalcScreenScrollX(cameraWorldX, tileIndex, tileCount, out.tileWidth, definition.parallax,definition.parallaxPixelScale);

        /* スケールを計算する。*/
        const float heightRatio = definition.screenHeightRatio > 0.0f ? definition.screenHeightRatio : 1.0f;
        const float displayH = screenH * heightRatio;

        /* テクスチャの縦横比を維持するためにスケールを計算する。*/
        out.scaleX = screenW / static_cast<float>(textureWidth);
        out.scaleY = displayH / static_cast<float>(textureHeight);

        /* Y 位置を計算する。*/
        if (definition.anchorBottom)
            out.screenY = -halfScreenH + displayH * 0.5f;
        else
            out.screenY = definition.screenCenterY;

        return out;
    }
} // namespace

namespace nsApp
{
    namespace nsStage
    {
        namespace nsScrollBackGround
        {
            bool ScrollLayerTile::Init(const ScrollLayerDefinition& definition, int tileIndex, nsK2EngineLow::Texture* pZprepassDepthTexture)
            {
                /* 初期化済みフラグをリセットする。*/
                isInitialized_ = false;
                tileIndex_ = tileIndex;

                /* テクスチャパスが無効な場合は初期化失敗とする。*/
                if (definition.texturePath == nullptr || definition.texturePath[0] == '\0')
                    return false;

                /* DDS ファイルからテクスチャを初期化する。*/
                wchar_t wDdsPath[512] = {};
                mbstowcs(wDdsPath, definition.texturePath, _countof(wDdsPath) - 1);
                texture_.InitFromDDSFile(wDdsPath);

                /* DDS ファイルが無効な場合は WIC ファイルからテクスチャを初期化する。*/
                if (!texture_.IsValid())
                    return false;

                /* テクスチャの幅と高さを取得する。*/
                const UINT texW = texture_.GetWidth();
                const UINT texH = texture_.GetHeight();
                if (texW == 0 || texH == 0)
                    return false;

                /* テクスチャの幅と高さをメンバ変数に保存する。*/
                textureWidth_ = texW;
                textureHeight_ = texH;
                pZprepassDepthTexture_ = pZprepassDepthTexture;

                /* スプライトを初期化する。*/
                SpriteInitData initData = {};
                initData.m_textures[0] = &texture_;
                initData.m_textures[1] = pZprepassDepthTexture_;
                initData.m_width = texW;
                initData.m_height = texH;
                initData.m_fxFilePath = kScrollBackGroundFxPath;
                initData.m_alphaBlendMode = AlphaBlendMode_Trans;
                initData.textureAddressMode = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
                initData.m_colorBufferFormat[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

                /* スプライトを初期化する。*/
                sprite_.Init(initData);
                sprite_.SetMulColor(Vector4::White);

                /* 初期化成功とする。*/
                worldPosition_ = Vector3::Zero;
                screenCenterX_ = 0.0f;
                screenTileWidth_ = 0.0f;
                isInitialized_ = true;
                return true;
            }


            void ScrollLayerTile::UpdateWorldPosition(float cameraWorldX, const ScrollLayerDefinition& definition)
            {
                /* 2D モード用の画面座標・スケールを更新する。*/
                ApplyScreenScrollLayout(cameraWorldX, definition);
            }


            void ScrollLayerTile::UpdateScreenPosition(float cameraWorldX, const ScrollLayerDefinition& definition)
            {
                /* 3D モード用の画面座標・スケールを更新する。*/
                ApplyScreenScrollLayout(cameraWorldX, definition);
            }


            void ScrollLayerTile::ApplyScreenScrollLayout(float cameraWorldX, const ScrollLayerDefinition& definition)
            {
                /* 初期化済みでない場合は処理を中断する。*/
                if (!isInitialized_)
                    return;

                /* レイヤーの表示位置とスケールを計算する。*/
                const auto xf =  CalcLayerDisplayTransform(definition, tileIndex_, cameraWorldX, textureWidth_, textureHeight_);

                /* 計算結果をメンバ変数に保存する。*/
                screenCenterX_ = xf.screenX;
                screenTileWidth_ = xf.tileWidth;
                worldPosition_.x = cameraWorldX + xf.screenX;

                /* スプライトの位置とスケールを更新する。*/
                const Vector3 pos(xf.screenX, xf.screenY, 0.0f);
                sprite_.Update(pos, Quaternion::Identity, Vector3(xf.scaleX, xf.scaleY, 1.0f));
            }


            void ScrollLayerTile::Draw(RenderContext& rc)
            {
                /* 初期化済みでない場合は描画を中断する。*/
                if (!isInitialized_)
                    return;

                /* スプライトを描画する。*/
                sprite_.Draw(rc);
            }


            bool ScrollLayerTile::IsVisible(float cameraWorldX, float viewHalfWidth, float margin, bool is3D) const
            {
                /* 未使用パラメータを抑制する。*/
                (void) cameraWorldX;
                (void) viewHalfWidth;
                (void) is3D;

                /* 初期化済みでない場合は非表示とする。*/
                if (!isInitialized_)
                    return false;

                /* カリング余白を設定する。*/
                const float cullMargin = margin > 0.0f ? margin : kDefaultCullMargin;
                const float halfScreenW = static_cast<float>(FRAME_BUFFER_W) * 0.5f;
                const float halfTile = screenTileWidth_ * 0.5f;
                const float left = screenCenterX_ - halfTile;
                const float right = screenCenterX_ + halfTile;

                /* タイルの左端が画面右端より左、かつタイルの右端が画面左端より右の場合は表示する。*/
                return right >= (-halfScreenW - cullMargin) && left <= (halfScreenW + cullMargin);
            }

        } // namespace nsScrollBackGround
    } // namespace nsStage
} // namespace nsApp
