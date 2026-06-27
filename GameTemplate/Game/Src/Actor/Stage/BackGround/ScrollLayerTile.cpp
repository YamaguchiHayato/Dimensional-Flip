#include "stdafx.h"

#include "Src/Actor/Stage/BackGround/ScrollLayerTile.h"
#include "Src/Core/CameraManager.h"

namespace
{
    using nsApp::nsStage::nsScrollBackGround::ScrollLayerDefinition;

    constexpr const char* kScrollBackGroundFxPath = "Assets/shader/ScrollBackGround.fx";
    constexpr float kDefaultCullMargin = 400.0f;
} // namespace

namespace nsApp
{
    namespace nsStage
    {
        namespace nsScrollBackGround
        {
            bool ScrollLayerTile::Init(const ScrollLayerDefinition& definition, int tileIndex,  nsK2EngineLow::Texture* pZprepassDepthTexture)
            {
                isInitialized_ = false;
                tileIndex_ = tileIndex;
                drawScale_ = definition.drawScale;

                if (definition.texturePath == nullptr || definition.texturePath[0] == '\0')
                    return false;

                wchar_t wDdsPath[512] = {};
                mbstowcs(wDdsPath, definition.texturePath, _countof(wDdsPath) - 1);
                texture_.InitFromDDSFile(wDdsPath);

                if (!texture_.IsValid())
                    return false;

                const UINT texW = texture_.GetWidth();
                const UINT texH = texture_.GetHeight();
                if (texW == 0 || texH == 0)
                    return false;

                // [修正] Init で幅を保存（Update の scale 計算用）
                textureWidth_ = texW;

                // [修正] 2D タイル幅は Config 優先
                if (definition.tileWorldWidth > 0.0f)
                    tileWorldWidth_ = definition.tileWorldWidth;
                else
                    tileWorldWidth_ = static_cast<float>(texW) * drawScale_;

                pZprepassDepthTexture_ = pZprepassDepthTexture;

                SpriteInitData initData = {};
                initData.m_textures[0] = &texture_;
                initData.m_textures[1] = pZprepassDepthTexture_; // [修正] ZPrepass 深度（Font と同じ t1）
                initData.m_width = texW;
                initData.m_height = texH;
                initData.m_fxFilePath = kScrollBackGroundFxPath; // [修正] ZPrepass 深度 discard 専用
                initData.m_alphaBlendMode = AlphaBlendMode_Trans;
                initData.textureAddressMode = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
                // [修正] mainRT フォーマット（Font と同様。必要なら ScrollStageBackGround から渡しても可）
                initData.m_colorBufferFormat[0] = DXGI_FORMAT_R8G8B8A8_UNORM;


                sprite_.Init(initData);
                sprite_.SetMulColor(Vector4::White);

                worldPosition_ = Vector3::Zero;
                screenCenterX_ = 0.0f;
                screenTileWidth_ = 0.0f;
                isInitialized_ = true;
                return true;
            }


            // [2D] 画面全体 + 横スクロール
            // [2D] 画面空間（中心原点）+ 横スクロール
            void ScrollLayerTile::UpdateWorldPosition(float cameraWorldX, const ScrollLayerDefinition& definition)
            {
                if (!isInitialized_)
                    return;

                const float screenW = static_cast<float>(FRAME_BUFFER_W);
                const float screenH = static_cast<float>(FRAME_BUFFER_H);
                const float halfScreenW = screenW * 0.5f;

                const float tileWidth = screenW;
                const int tileCount = definition.tileCount > 0 ? definition.tileCount : 1;
                const float loopLength = tileWidth * static_cast<float>(tileCount);

                const float scrollOffset = cameraWorldX * definition.parallax;

                float phase = std::fmod(scrollOffset, loopLength);
                if (phase < 0.0f)
                    phase += loopLength;

                // [修正] 中心原点。phase=0 で tile0 の中心が画面中心(0,0)
                const float screenX =
                    -phase + static_cast<float>(tileIndex_) * tileWidth + tileWidth * 0.5f - halfScreenW;

                screenCenterX_ = screenX;
                screenTileWidth_ = tileWidth;

                // カリング用（ワールド X は従来どおり保持）
                worldPosition_.x = cameraWorldX + screenX;
                worldPosition_.y = 0.0f;
                worldPosition_.z = definition.worldZ;

                const float scaleX = screenW / static_cast<float>(textureWidth_);
                const float scaleY = screenH / static_cast<float>(texture_.GetHeight());

                const Vector3 pos(screenX, 0.0f, 0.0f);
                sprite_.Update(pos, Quaternion::Identity, Vector3(scaleX, scaleY, 1.0f));
            }

            // [3D] とりあえずスクロール無し・画面全体
            // [3D] スクロール無し・画面全体（中心原点）
            void ScrollLayerTile::UpdateScreenPosition(float cameraWorldX, const ScrollLayerDefinition& definition)
            {
                (void) cameraWorldX;
                (void) definition;

                if (!isInitialized_)
                    return;

                const float screenW = static_cast<float>(FRAME_BUFFER_W);
                const float screenH = static_cast<float>(FRAME_BUFFER_H);

                screenCenterX_ = 0.0f;
                screenTileWidth_ = screenW;

                const float scaleX = screenW / static_cast<float>(textureWidth_);
                const float scaleY = screenH / static_cast<float>(texture_.GetHeight());

                const Vector3 pos(0.0f, 0.0f, 0.0f);
                sprite_.Update(pos, Quaternion::Identity, Vector3(scaleX, scaleY, 1.0f));
            }

            void ScrollLayerTile::Draw(RenderContext& rc)
            {
                if (!isInitialized_)
                    return;

                // [修正] Update 済み。ScrollBackGround.fx + Draw(rc)（Font と同経路）
                sprite_.Draw(rc);
            }


            bool ScrollLayerTile::IsVisible(float cameraWorldX, float viewHalfWidth, float margin, bool is3D) const
            {
                (void) cameraWorldX;
                (void) viewHalfWidth;

                if (!isInitialized_)
                    return false;

                const float cullMargin = margin > 0.0f ? margin : kDefaultCullMargin;
                const float halfScreenW = static_cast<float>(FRAME_BUFFER_W) * 0.5f;
                const float halfTile = screenTileWidth_ * 0.5f;
                const float left = screenCenterX_ - halfTile;
                const float right = screenCenterX_ + halfTile;

                // [修正] 2D/3D とも中心原点の画面 X で判定
                return right >= (-halfScreenW - cullMargin) && left <= (halfScreenW + cullMargin);
            }
        } // namespace nsScrollBackGround
    } // namespace nsStage
} // namespace nsApp
