#pragma once

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

            void DestryCollision();

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
            ModelRender render_;
            Vector3 pos_;
            Quaternion rot_;

        };

    }

}
