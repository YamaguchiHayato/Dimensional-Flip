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

        public:
            inline void SetPos(const Vector3& pos) { pos_ = pos; }
            inline const Vector3 GetPos() const { return pos_; }

        private:
            // 物理を作り直す関数
            void RefreshPhysics();

        private:
            ModelRender render_;

            // ★変更点: 作り直せるようにポインタ(*)にします
            PhysicsStaticObject* pPhysics_ = nullptr;

            Player* pPlayer_ = nullptr;
            Vector3 pos_ = Vector3::Zero;

            // --- 大きさの設定 ---
            // 3Dモード時: 普通の大きさ（薄い状態）
            Vector3 scale_3D_Normal = Vector3(0.1f, 0.1f, 0.1f);

            // 2Dモード時: 通せんぼ用の大きさ（画像の分厚い状態）
            Vector3 scale_2D_Wide = Vector3(0.1f, 0.1f, 2.0f);

            Vector3 currentScale_ = Vector3::One;
            Quaternion rot_ = Quaternion::Identity;

            bool is2DMode_ = false;
        };
    } // namespace stage
} // namespace app
