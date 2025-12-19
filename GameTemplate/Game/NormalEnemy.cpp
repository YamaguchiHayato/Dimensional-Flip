#include "stdafx.h"
#include "NormalEnemy.h"
#include "Src/Actor/Character/Player.h"

namespace app
{
    namespace enemy
    {
        bool NormalEnemy::Start()
        {
            render_.Init("Assets/modelData/enemy/boss.tkm");


            initPos_ = pos_;

            // 座標をセット。
            render_.SetPosition(pos_);

            // 更新処理。
            render_.Update();
            bigEnemy.Update();

            pPlayer_ = FindGO<Player>("player");


            if (scale_.x > 1.0f)
            {

                Vector3 collisionSize(100.0f, 500.0f, 50.0f);

                // 座標オフセット（足元から上に伸ばすイメージ）
                Vector3 offset(0.0f, 250.0f, 0.0f);

            }
            return true;
        }


        void NormalEnemy::Update()
        {
            if (!pPlayer_)
                return;

            Vector3 diff = pPlayer_->GetPlayerPos() - pos_;
            float distSq = diff.LengthSq();

            if (distSq <= radius_ * radius_)
            {
                Vector3 playerVer = pPlayer_->GetMoveSpeed();


                const float dy = pPlayer_->GetPlayerPos().y - pos_.y;
                if (isStompable_ && playerVer.y < 0.0f && dy > 0.0f)
                {
                    // プレイヤーを跳ね返す。
                    pPlayer_->Bound();

                    // 自身を削除。
                    DeleteGO(this);
                    return;
                }

                // @TODO playerがダメージを受ける処理。

            }

            render_.SetScale(scale_);
            render_.SetPosition(pos_);
            render_.Update();


        }


        void NormalEnemy::Press()
        {
            if (isCrushed_)
            {
                crushedFrame_++;

                // 0～1に正規化
                float t = (crushedFrame_ >= kCrushFrames) ? 1.0f : (crushedFrame_ / (float) kCrushFrames);

                // Yだけ潰す（＋少し沈める）
                const float startY = crushStartScale_.y;
                const float endY = crushStartScale_.y * crushScaleYRate_;
                scale_.y = startY + (endY - startY) * t;

                const float startPosY = crushStartPos_.y;
                const float endPosY = crushStartPos_.y - crushPosYOffset_;
                pos_.y = startPosY + (endPosY - startPosY) * t;

                // 潰れ終わって少し待ったら削除
                if (crushedFrame_ >= kCrushFrames + kCrushWaitFrames)
                {
                    DeleteGO(this);
                    return;
                }
            }
        }

        void NormalEnemy::Render(RenderContext& rc)
        {
            render_.Draw(rc);
        }
    }
}
