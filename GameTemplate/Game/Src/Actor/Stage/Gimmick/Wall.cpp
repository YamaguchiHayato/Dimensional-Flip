#include "stdafx.h"

#include "Src/Actor/Character/Player.h"
#include "Src/Actor/Stage/Gimmick/Wall.h"
#include "Src/Collision/CollisionManager.h"

namespace
{
    const Vector3 BOX_BASE_HALF(150.0f, 400.0f, 500.0f);

    inline Vector3 CalcCenterOffset(const Vector3& halfExtents)
    {
        return Vector3(0.0f, halfExtents.y, 0.0f);
    }
    constexpr auto COLLISION_PROPERTY = app::collision::CollisionProperty::SolidOnly2D;
} // namespace



namespace app
{
    namespace stage
    {
        Wall::~Wall()
        {
            DestroyCollision();
        }


        bool Wall::Start()
        {
            pPlayer_ = FindGO<Player>("player");
            initPos_ = pos_;

            // 見た目
            render_.Init("Assets/stage/Stage1/Wall.tkm");
            render_.SetScale(scale_);
            render_.SetPosition(pos_);
            render_.SetRotation(rot_);
            render_.Update();


            wallPhysics_.CreateFromModel(render_.GetModel(), render_.GetModel().GetWorldMatrix());

            // コリジョン。
            pCollision_ = NewGO<CollisionObject>(0);
            // コリジョンのモード設定。
            pCollision_->SetCollisionProperty(COLLISION_PROPERTY);

            //　コリジョンの形状。
            const Vector3 halfExtents
            (
                BOX_BASE_HALF.x * scale_.x,
                BOX_BASE_HALF.y * scale_.y,
                BOX_BASE_HALF.z * scale_.z
            );

            const Vector3 centerOffset = CalcCenterOffset(halfExtents);
            pCollision_->CreateBox(pos_ + centerOffset, rot_, halfExtents);
            pCollision_->SetIsEnableAutoDelete(false);
            return true;
        }


        void Wall::Update()
        {
            // コリジョンマネージャの現在の状態を受け取る。
            // collision … SingletonInstance。
            auto mode = collision::CollisionManager::GetInstance().GetCurrentMode();

            // モードに応じた通行の有無。
            if (mode == collision::CollisionManager::GetInstance().GetCurrentMode())
                render_.SetAlpha(0.5f);

            else
                render_.SetAlpha(1.0f) ;


            // 見た目更新
            render_.SetScale(scale_);
            render_.SetPosition(pos_);
            render_.SetRotation(rot_);
            render_.Update();


            if (pCollision_)
            {
                const Vector3 halfExtents
                (
                    BOX_BASE_HALF.x * scale_.x,
                    BOX_BASE_HALF.y * scale_.y,
                    BOX_BASE_HALF.z * scale_.z
                );


                pCollision_->SetPosition(pos_ + CalcCenterOffset(halfExtents));
            }
        }


        void Wall::Render(RenderContext& rc)
        {
            render_.Draw(rc);
        }


        void Wall::DestroyCollision()
        {
            if (!isCollisionActive_)
                return;

            if (pCollision_)
            {
                collision::CollisionManager::GetInstance().UnRegisterObject(pCollision_);
                DeleteGO(pCollision_);
                pCollision_ = nullptr;
            }

            wallPhysics_.Release();
            isCollisionActive_ = false;
        }
    } // namespace stage
} // namespace app
