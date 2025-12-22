#include "stdafx.h"
#include "NormalEnemy.h"
#include "Src/Actor/Character/Player.h"

namespace
{
    static constexpr uint8_t kCrushFrames = 6;      // 潰れるまでのフレーム数
    static constexpr uint8_t kCrushWaitFrames = 10; // 潰れたまま残すフレーム数

}


namespace app
{
    namespace enemy
    {
        bool NormalEnemy::Start()
        {
//            render_.Init("Assets/modelData/enemy/boss.tkm");
            render_.Init("Assets/modelData/enemy/umbrella_yellow.tkm");


            // 座標をセット。
            render_.SetPosition(pos_);
            // 更新処理。
            render_.Update();

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
            if (isCrushed_)
            {
                Press();
                return;
            }

            // --- 2. 通常時の処理 ---
            if (!pPlayer_)
                return;


            Vector3 diff = pPlayer_->GetPlayerPos() - pos_;
            float distSq = diff.LengthSq();


            if (distSq <= radius_ * radius_)
            {
                Vector3 playerVer = pPlayer_->GetMoveSpeed();

                const float dy = pPlayer_->GetPlayerPos().y - pos_.y;
                // プレイヤーが落下中 かつ 敵より上にいる
                if (isStompable_ && playerVer.y < 0.0f && dy > 0.0f)
                {
                    pPlayer_->Bound();

                    // フラグを立てる
                    isCrushed_ = true;
                    crushedFrame_ = 0;

                    // 開始時の情報を保存
                    crushStartScale_ = scale_;
                    crushStartPos_ = pos_;

                    // 当たり判定を消す
                    radius_ = 0.0f;

                    return;
                }
            }

            render_.SetScale(scale_);
            render_.SetPosition(pos_);
            render_.Update();
        }


        void NormalEnemy::Press()
        {
            // 経過フレームを進める
            crushedFrame_++;

            // 進行度 t を 0.0 ～ 1.0 で計算
            float t = (float) crushedFrame_ / (float) kCrushFrames;
            if (t > 1.0f)
                t = 1.0f;

            // スケール計算
            const float startY = crushStartScale_.y;
            const float endY = crushStartScale_.y * crushScaleYRate_;
            scale_.y = startY + (endY - startY) * t;

            // 座標計算
            const float startPosY = crushStartPos_.y;
            const float endPosY = crushStartPos_.y - crushPosYOffset_;
            pos_.y = startPosY + (endPosY - startPosY) * t;

            render_.SetScale(scale_);
            render_.SetPosition(pos_);
            render_.Update();

            // アニメーション完了＋待機時間が過ぎたら削除
            if (crushedFrame_ >= kCrushFrames + kCrushWaitFrames)
                DeleteGO(this);
        }


        void NormalEnemy::InitParam(const SpawnParam& param)
        {
            // 座標を保持。
            pos_ = param.paramPos_;

            // 大きさを保持。
            scale_ = param.paramScale;

            //
            if (param.paramCollision > 0.0f)
            {
                scale_ = param.paramScale;
                radius_ = param.paramCollision;
            }

            else
            {
                scale_ = param.paramScale;
            }

            isStompable_ = param.paramFlag;

            // レンダーの更新。
            render_.SetPosition(pos_);
            render_.SetScale(scale_);
        }


        void NormalEnemy::Render(RenderContext& rc)
        {
            render_.Draw(rc);
        }
    }
}
