#include "stdafx.h"

#include "Switch.h"
// Playerの座標を知るために必須
#include "Src/Actor/Character/Player/Player.h"

namespace app
{
    namespace gimmick
    {
        // 判定の中心（丸い部分は土台の上にあるので、少し上にずらす）
        const Vector3 COLLISION_OFFSET(0.0f, 30.0f, 0.0f);

        // 反応する距離（50.0f 以内に近づいたらON）
        const float TRIGGER_DISTANCE = 30.0f;

        bool Switch::Start()
        {
            render_.Init("Assets/stage/Stage1/switchGimmick.tkm");
            render_.SetScale(Vector3(0.5f, 0.5f, 0.5f));

            // コリジョンオブジェクト（CollisionObject）は削除します
            // 物理的な当たり判定は使わず、計算だけで処理するため不要です

            Initialize();
            return true;
        }

        void Switch::Initialize()
        {
            isPressed_ = false;
            isTriggered_ = false;
            originalScale_ = Vector3(0.5f, 0.5f, 0.5f);
        }

        void Switch::Update()
        {
            // 既に押されていたら何もしない
            if (isPressed_)
                return;

            render_.SetPosition(gimmickPos_);
            render_.Update();


            // 1. プレイヤーを探す
            Player* player = FindGO<Player>("player");

            // 見つからない場合、念のため大文字も試す
            if (player == nullptr)
            {
                player = FindGO<Player>("Player");
            }

            if (player != nullptr)
            {
                // 2. 座標を取得
                Vector3 playerPos = player->GetPlayerPos();

                // 丸い部分の中心座標を計算
                Vector3 switchCenter = gimmickPos_ + COLLISION_OFFSET;

                // 3. 距離を計算（ベクトルの長さ）
                Vector3 diff = playerPos - switchCenter;
                float distance = diff.Length();

                // 4. 設定した距離より近づいたら「押した」ことにする
                if (distance < TRIGGER_DISTANCE)
                {
                    // printf("Switch Triggered! Dist: %f\n", distance);
                    Press();
                }
            }
        }

        void Switch::Render(RenderContext& rc)
        {
            if (isPressed_)
                return;
            render_.Draw(rc);
        }

        void Switch::OnCollisionEnter(CollisionObject* object)
        {
        }

        void Switch::Press()
        {
            if (isPressed_)
                return;

            isPressed_ = true;
            isTriggered_ = true;

            // 押されたら姿を消す
            render_.SetScale(Vector3::Zero);
        }

        bool Switch::GetIsPressedTrigger()
        {
            if (isTriggered_)
            {
                isTriggered_ = false;
                return true;
            }
            return false;
        }
    } // namespace gimmick
} // namespace app
