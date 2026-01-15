#include "stdafx.h"

#include "Src/Actor/Character/Enemy/Fall/FallEnemyRiseState.h"

namespace
{
    const float DELTA_TIME = 1.0f / 60.0f;
}

namespace app
{
    namespace enemy
    {

        void FallEnemyRiseState::Enter()
        {
            isArrived_ = false;
        }

        void FallEnemyRiseState::Update()
        {
            // 敵クラスの安全チェック。
            if (!pFallEnemy_)
                return;

            // 現在座標取得
            Vector3 pos = pFallEnemy_->GetPosition();
            // 開始位置の取得。
            Vector3 startPos = pFallEnemy_->GetStartPos();

            // 座標のX座標とZ座標をあわせる。
            pos.x = startPos.x;
            pos.z = startPos.z;

            // 移動処理 (上方向へゆっくり)
            pos.y += pFallEnemy_->GetRiseSpeed() * DELTA_TIME;

            // 初期位置（天井）に戻ったか判定
            float startY = pFallEnemy_->GetStartPos().y;

            if (pos.y >= startY)
            {
                pos.y = startY; // 補正
                isArrived_ = true;
            }

            pFallEnemy_->SetPos(pos);
        }

        void FallEnemyRiseState::Exit()
        {
            // 念のため位置を確実に合わせる
            Vector3 pos = pFallEnemy_->GetStartPos();
            pos.y = pFallEnemy_->GetStartPos().y;
            pFallEnemy_->SetPos(pos);
        }


        bool FallEnemyRiseState::RequestID(uint8_t& request)
        {
            if (isArrived_)
            {
                request = static_cast<uint8_t>(FallEnemyState::state_Idle);
                return true;
            }
            return false;
        }
    } // namespace enemy
} // namespace app
