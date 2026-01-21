#include "stdafx.h"

#include "NormalCrushedState.h"
#include "Src/Actor/Character/Enemy/Normal/NormalEnemy.h"

namespace
{
    // アニメーション定数
    const uint8_t CRUSH_FRAMES = 6;
    const uint8_t CRUSH_WAIT_FRAMES = 10;
} // namespace

namespace app
{
    namespace enemyState
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

            if (pNormal_->crushedFrame_ <= CRUSH_FRAMES)
            {
                float t = static_cast<float>(pNormal_->crushedFrame_) / static_cast<float>(CRUSH_FRAMES);

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

            // --- 完了後の待機 & 処理 ---
            pNormal_->UpdateRender();

            if (pNormal_->crushedFrame_ >= CRUSH_FRAMES + CRUSH_WAIT_FRAMES)
            {
                // 1. 大きさを0にして見えなくする
                pNormal_->SetScale(Vector3::Zero);

                // 2. 場所をはるか彼方に飛ばして、当たり判定などを無効化する
                pNormal_->SetPos(Vector3(0.0f, -5000.0f, 0.0f));

                // 3. 変更を反映させる
                pNormal_->UpdateRender();
            }
        }
        void NormalCrushedState::Exit() {}


        bool NormalCrushedState::RequestID(uint8_t& request)
        {
            return false;
        }
    } // namespace enemy
} // namespace app
