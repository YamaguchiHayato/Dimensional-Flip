#include "stdafx.h"
#include "Src/Actor/Stage/Gimmick/FloatingPlatform.h"

namespace
{
    const auto SPAWN_DEPTH = -300.0f; // 地中の深さ
    const Vector3 COLLISION_SIZE = Vector3(150.0f, 20.0f, 40.0f);

    const Vector3 GIMMICKCOLLISION(0.0f, 0.0f, 0.0f); // コリジョンの高さ

} 

namespace app
{
    namespace gimmick
    {
        bool FloatingPlatform::Start()
        {
            const std::string path = InitGimmick("StageEX/pole");
            render_.Init(path.c_str());

            isActive_ = false;
            currentPos_ = Vector3::Zero;
            targetPos_ = Vector3::Zero;

            render_.SetScale(Vector3::One);
            render_.SetPosition(currentPos_);
            render_.Update();

            // 物理。
            gimmickPhysics_.CreateFromModel(render_.GetModel(), render_.GetModel().GetWorldMatrix());

            // コリジョン。
            pGimmickCollision_ = NewGO<CollisionObject>(0, "collisionobject");
            // コリジョンを動く床に設置
            pGimmickCollision_->CreateBox(currentPos_, Quaternion::Identity, COLLISION_SIZE);

            // コリジョンを破棄しない設定。
            pGimmickCollision_->SetIsEnableAutoDelete(false);
            return true;
        }


        void FloatingPlatform::Update()
        {

            // 非アクティブ状態なら更新しない。
            if (!isActive_)
                return;

            //少しずつ浮上させて、目標に向かわせる。
            if (currentPos_.y < targetPos_.y)
            {
                currentPos_.y += 5.0f; // 上昇速度
                if (currentPos_.y > targetPos_.y)
                    currentPos_.y = targetPos_.y;
            }

            render_.SetScale(Vector3::One);
            // ギミックモデルの更新。
            render_.Update();
            // ギミックモデルの座標更新。
            render_.SetPosition(currentPos_);
            // ギミック物理の座標更新。
            gimmickPhysics_.SetPosition(currentPos_);
            // ギミックコリジョンの設定。
            pGimmickCollision_->SetPosition(currentPos_);

            if (pGimmickCollision_)
                pGimmickCollision_->SetPosition(currentPos_);
        }


        void FloatingPlatform::Render(RenderContext& rc)
        {
            if (!isActive_)
                return;

            render_.Draw(rc);
        }


        void FloatingPlatform::Activate(const Vector3& pos)
        {
            isActive_ = true;
            targetPos_ = pos;

            // 地中からスタート
            currentPos_ = pos;
            currentPos_.y += SPAWN_DEPTH;

            render_.SetPosition(currentPos_);

            // 出現時に座標を同期する。
            if (pGimmickCollision_)
                pGimmickCollision_->SetPosition(currentPos_);

            gimmickPhysics_.SetPosition(currentPos_);
        }
    }
}
