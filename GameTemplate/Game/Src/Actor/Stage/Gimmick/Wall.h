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
            virtual ~Wall() = default;

            bool Start();
            void Update();
            void Render(RenderContext& rc);

            void DestroyCollision();

        // セッター。
        public:
            inline void SetPos(const Vector3& pos)
            {
                pos_ = pos;
            }


        // ゲッター。
        public:
            // Playerがカメラアクションをするとこのオブジェクトのコリジョンを消す関数。
            inline const Vector3 GetPos() const
            {
                return pos_;
            }

        private:
            CollisionObject* pCollision_ = nullptr;
            Player* pPlayer_ = nullptr;
        private:
            ModelRender render_;

            Vector3 pos_;
            Vector3 initPos_;

            PhysicsStaticObject wallPhysics_;
            Quaternion rot_;

            // 多重削除防止用フラグ。
            bool isCollisionActive_ = true;
        };

    }

}
