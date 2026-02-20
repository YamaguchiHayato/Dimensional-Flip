#include "stdafx.h"

#include "Coin.h"
#include "Src/Actor/Character/Player/Player.h"

namespace app
{
    namespace gimmick
    {
        // コインの取得範囲（広めにとっておくと気持ちいいです）
        const float COLLECT_DISTANCE =8.0f;

        bool Coin::Start()
        {
            render_.Init("Assets/stage/Stage1/coin.tkm");
            render_.SetScale(Vector3(0.25f,0.25f, 0.25f));

            isVisible_ = false;
            isCollected_ = false;

            // コリジョンオブジェクトは削除（距離判定にするため）

            return true;
        }

        void Coin::Update()
        {
            // 出現していない、または既に取られていたら処理しない
            if (!isVisible_ || isCollected_)
                return;

            // 回転演出
            rotateAngle_ += 100.0f * g_gameTime->GetFrameDeltaTime();
            Quaternion rot;
            rot.SetRotationDegY(rotateAngle_);
            render_.SetRotation(rot);
            render_.SetPosition(gimmickPos_);
            render_.Update();

            // ★修正：距離判定
            Player* player = FindGO<Player>("player");
            if (player == nullptr)
                player = FindGO<Player>("Player");

            if (player != nullptr)
            {
                Vector3 playerPos = player->GetPlayerPos();

                // コインとプレイヤーの距離
                Vector3 diff = playerPos - gimmickPos_;

                if (diff.Length() < COLLECT_DISTANCE)
                {
                    // 取得処理
                    isCollected_ = true;
                    getTrigger_ = true;
                }
            }
        }

        void Coin::Render(RenderContext& rc)
        {
            if (!isVisible_ || isCollected_)
                return;
            render_.Draw(rc);
        }

        void Coin::OnCollisionEnter(CollisionObject* object) {}

        void Coin::Appear()
        {
            if (isVisible_)
                return;
            isVisible_ = true;
            isCollected_ = false;
        }

        bool Coin::GetIsCollectedTrigger()
        {
            if (getTrigger_)
            {
                getTrigger_ = false;
                return true;
            }
            return false;
        }
    } // namespace gimmick
} // namespace app
