#include "stdafx.h"
#include <cstring> 

#include "BackGroundLayer.h"
#include "graphics/Sprite.h" 

namespace app
{
    namespace stage
    {
        BackGroundLayer::BackGroundLayer()
        {
            pSprite_ = new Sprite();

            std::memset(&cbData_, 0, sizeof(cbData_));
            cbData_.linearWipeDrawingMode = -1;
            cbData_.screenDrawingMode = -1;
        }


        BackGroundLayer::~BackGroundLayer()
        {
            if (pSprite_)
            {
                delete pSprite_;
                pSprite_ = nullptr;
            }
        }


        void BackGroundLayer::Init(const char* filePath, float zDepth, float scrollRatio)
        {
            zDepth_ = zDepth;
            scrollRaito_ = scrollRatio;

  
            SpriteInitData initData;
            // 画像をロード
            initData.m_ddsFilePath[0] = filePath;
            // 画像のサイズを設定する。
            initData.m_width = 1920;
            initData.m_height = 1080;


            // 定数バッファ等の設定
            initData.m_expandConstantBuffer = &cbData_;
            initData.m_expandConstantBufferSize = sizeof(cbData_);
            initData.textureAddressMode = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
            initData.m_fxFilePath = "Assets/shader/sprite.fx";
            pSprite_->Init(initData);

            pSprite_->SetMulColor({1.0f, 1.0f, 1.0f, 1.0f});
        }


        void BackGroundLayer::Update(const Vector3& cameraPos)
        {
            Vector3 bgPos = cameraPos;
            bgPos.z = cameraPos.z + zDepth_;

            Quaternion rot;
            rot.SetRotationY(Math::PI);

            // 回転を適用して更新
            pSprite_->Update(bgPos, rot, {1.0f, 1.0f, 1.0f});

            // UVスクロールの計算
            float scrollSpeed = scrollRaito_ * 0.001f;
            cbData_.uvOffset.x = cameraPos.x * scrollSpeed;
            cbData_.uvOffset.y = 0.0f;
        }


        void BackGroundLayer::Draw(RenderContext& rc, nsK2EngineLow::Camera* pCamera)
        {
            if (pCamera)
                // 3Dモード：ワールド空間に置いて描く
                pSprite_->Draw(rc, pCamera);


            else
                // 2Dモード：画面に貼り付けて描く
                pSprite_->Draw(rc);
        }
    } // namespace stage
} // namespace app
