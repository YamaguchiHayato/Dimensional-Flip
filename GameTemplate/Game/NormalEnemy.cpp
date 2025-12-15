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
            charaCon_.Init(20.0f, 25.0f, pos_);

            // 座標をセット。
            render_.SetPosition(pos_);

            // 更新処理。
            render_.Update();

            pPlayer_ = FindGO<Player>("player");
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

                if (playerVer.y < 0.0f && pPlayer_->GetPlayerPos().y - pos_.y)
                {
                    // プレイヤーを跳ね返す。
                    pPlayer_->Bound();

                    // 自身を削除。
                    DeleteGO(this);
                }

                // @TODO playerがダメージを受ける処理。

            }

            render_.SetScale(Vector3(0.25f, 0.25f, 0.25f));
            render_.SetPosition(pos_);
            render_.Update();
        }


        void NormalEnemy::Render(RenderContext& rc)
        {
            render_.Draw(rc);
        }
    }
}
