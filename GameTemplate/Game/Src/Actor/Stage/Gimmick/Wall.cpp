#include "stdafx.h"
#include "Src/Collision/CollisionManager.h"

#include "Src/Actor/Character/Player.h"
#include "Src/Actor/Stage/Gimmick/Wall.h"

namespace
{
    const Vector3 COLLISION_BOX_SIZE(200.0f, 700.0f, 700.0f);
    const Vector3 COLLISION_OFFSET(0.0f, 500.0f, 0.0f);
} 

namespace app
{
    namespace stage
    {
        Wall::~Wall()
        {
            // コリジョンの削除。
            DestroyCollision();
        }



        bool Wall::Start()
        {
            pPlayer_ = FindGO<Player>("player");

            rot_ = Quaternion::Identity;

            render_.Init("Assets/stage/Stage1/Wall.tkm");
            render_.SetScale(scale_);
            render_.SetPosition(pos_);
            render_.SetRotation(rot_);
            render_.Update();


            // 2. 箱型のコリジョン (CollisionObject)
            pCollision_ = NewGO<CollisionObject>(0, "collisionobject");

            // 各座標成分に乗算をしてサイズを指定する。
            Vector3 finalSize = {COLLISION_BOX_SIZE.x * scale_.x, COLLISION_BOX_SIZE.y * scale_.y,
                                 COLLISION_BOX_SIZE.z * scale_.z};

            pCollision_->CreateBox(pos_ +COLLISION_OFFSET, Quaternion::Identity, finalSize);
            pCollision_->SetIsEnableAutoDelete(false);

            // コリジョンマネージャーに2D時のみ有効として登録する。
            collision::CollisionManager::GetInstance().RegisterObject(pCollision_,collision::CollisionProperty::SolidOnly2D);

            isCollisionActive_ = true;

            return true;
        }

        void Wall::Update()
        {
            // 見た目の更新
            Quaternion rot;
            render_.SetRotation(rot_);
            render_.SetPosition(pos_);
            render_.SetScale(scale_); // メンバ変数のスケールを適用
            render_.Update();

            if (pCollision_)
            {
                pCollision_->SetPosition(pos_ + COLLISION_OFFSET);
                pCollision_->SetRotation(rot_);
            }
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
                    // コリジョンを削除する前にコリジョンの登録を解除する。
                    collision::CollisionManager::GetInstance().UnRegisterObject(pCollision_);

                    DeleteGO(pCollision_);
                    pCollision_ = nullptr;
                }

                wallPhysics_.Release();
                isCollisionActive_ = false;
            }
        }
    } 
} 
