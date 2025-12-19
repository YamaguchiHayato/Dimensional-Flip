#include "stdafx.h"

#include "Src/Actor/Character/Player.h"
#include "Src/Actor/Stage/Gimmick/Wall.h"

namespace
{
    const Vector3 COLLISION_BOX_SIZE(500.0f, 500.0f, 500.0f); // 名前変えました: サイズ
    const Vector3 COLLISION_OFFSET(0.0f, 200.0f, 0.0f);       // 名前変えました: オフセット
}

namespace app
{
    namespace stage
    {
        bool Wall::Start()
        {
            pPlayer_ = FindGO<Player>("player");

            render_.Init("Assets/stage/DimensionWall.tkm");
            render_.SetScale(Vector3(2.0f, 2.0f, 2.0f));
            render_.SetPosition(pos_);
            render_.Update(); // マトリクス更新

            // 1. モデル形状の物理判定 (PhysicsStaticObject)
            wallPhysics_.CreateFromModel(render_.GetModel(), render_.GetModel().GetWorldMatrix());

            // 2. 箱型のコリジョン (CollisionObject)
            pCollision_ = NewGO<CollisionObject>(0, "collisionobject");
            // 作成時は「座標 + オフセット」
            pCollision_->CreateBox(pos_ + COLLISION_OFFSET, Quaternion::Identity, COLLISION_BOX_SIZE);
            pCollision_->SetIsEnableAutoDelete(false);

            isCollisionActive_ = true;

            return true;
        }


        void Wall::Update()
        {
            // PlayerのAction判定
            if (isCollisionActive_ && pPlayer_)
            {
                if (pPlayer_->IsDimensionSwitchAction())
                {
                    Vector3 diff = pPlayer_->GetPlayerPos() - pos_;

                    // 距離チェック (半径250以内)
                    if (diff.LengthSq() < 250.0f * 250.0f)
                    {
                        DestroyCollision();
                    }
                }
            }

            // 見た目の更新
            Quaternion rot;
            rot.SetRotationDeg(Vector3::AxisY, 90.0f);
            render_.SetRotation(rot);
            render_.SetPosition(pos_);
            render_.SetScale(Vector3(2.0f, 2.0f, 2.0f));
            render_.Update();

            wallPhysics_.SetPosition(pos_);

            if (pCollision_)
                pCollision_->SetPosition(pos_ + COLLISION_OFFSET);
        }


        void Wall::Render(RenderContext& rc)
        {
            render_.Draw(rc);
        }


        void Wall::DestroyCollision()
        {
            if (isCollisionActive_)
            {
                if (pCollision_)
                {
                    DeleteGO(pCollision_);
                    pCollision_ = nullptr;
                }

                wallPhysics_.Release();
                isCollisionActive_ = false;
            }
        }
    } 
} 
