#pragma once

class Player;

namespace app
{
    namespace stage
    {
        class Wall : public IGameObject
        {
        public:
            Wall() = default;
            virtual ~Wall();

            bool Start();
            void Update();
            void Render(RenderContext& rc);
            void DestroyCollision();

            void SetPos(const Vector3& pos) { pos_ = pos; }
            const Vector3 GetPos() const { return pos_; }

        private:
            // 2D側（CollisionManagerで使われる）
            CollisionObject* pCollision_ = nullptr;

            // 3D側（キャラコンが物理を見るならこれが必要）
            PhysicsStaticObject wallPhysics_;

            ModelRender render_;
            Player* pPlayer_ = nullptr;

            Vector3 pos_ = Vector3::Zero;
            Vector3 initPos_ = Vector3::Zero;


            Vector3 collisionHalfWorld_ = Vector3(20.0f, 30.0f, 50.0f); // 半径（HalfExtents）
            Vector3 collisionOffsetWorld_ = Vector3(0.0f, 30.0f, 0.0f); // 中心位置オフセット

            // 見た目（Wallモデルに合わせてる前提）
            Vector3 scale_ = Vector3(0.1f, 0.1f, 0.1f);
            Quaternion rot_ = Quaternion::Identity;

            bool isCollisionActive_ = false;
        };
    } // namespace stage
} // namespace app
