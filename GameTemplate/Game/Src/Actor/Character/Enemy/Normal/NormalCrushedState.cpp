#include "stdafx.h"

#include "NormalCrushedState.h"
#include "Src/Actor/Character/Enemy/Normal/NormalEnemy.h"

namespace
{
    // アニメーション定数
    const uint8_t K_CRUSH_FRAMES = 6;
    const uint8_t K_CRUSH_WAIT_FRAMES = 10;
} // namespace

namespace app
{
    namespace enemy
    {
        void NormalCrushedState::Enter()
        {
            if (!pNormal_)
                return;

            // --- 死亡開始の初期化 (friendアクセス) ---
            pNormal_->isCrushed_ = true;
            pNormal_->crushedFrame_ = 0;
            pNormal_->crushStartScale_ = pNormal_->scale_; // 現在のスケール保存
            pNormal_->crushStartPos_ = pNormal_->pos_;     // 現在の位置保存
        }

        void NormalCrushedState::Update()
        {
            if (!pNormal_)
                return;

            // --- 潰れアニメーション実行 ---
            pNormal_->crushedFrame_++;

            if (pNormal_->crushedFrame_ <= K_CRUSH_FRAMES)
            {
                float t = static_cast<float>(pNormal_->crushedFrame_) / static_cast<float>(K_CRUSH_FRAMES);

                // Yスケールを縮める
                float startY = pNormal_->crushStartScale_.y;
                float endY = startY * pNormal_->crushScaleYRate_;
                pNormal_->scale_.y = startY + (endY - startY) * t;

                // Y位置を下げる（地面にめり込ませる）
                float startPosY = pNormal_->crushStartPos_.y;
                float endPosY = startPosY - pNormal_->crushPosYOffset_;
                pNormal_->pos_.y = startPosY + (endPosY - startPosY) * t;

                pNormal_->UpdateRender();
                return;
            }

            // --- 完了後の待機 & 削除 ---
            // 完全に潰れたら当たり判定などを消す処理があればここに記述
            pNormal_->UpdateRender();

            if (pNormal_->crushedFrame_ >= K_CRUSH_FRAMES + K_CRUSH_WAIT_FRAMES)
            {
                DeleteGO(pNormal_);
            }
        }

        void NormalCrushedState::Exit() {}

        bool NormalCrushedState::RequestID(uint8_t& request)
        {
            // 死亡したら他の状態には戻らない
            return false;
        }
    } // namespace enemy
} // namespace app
