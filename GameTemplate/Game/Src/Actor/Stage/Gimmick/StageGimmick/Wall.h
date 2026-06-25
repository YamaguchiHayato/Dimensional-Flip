#pragma once
#include "Src/UI/ButtonActionUI.h"
#include "Src/Actor/Character/PlayerForward.h"


namespace app
{
    namespace stage
    {
        class Wall : public IGameObject
        {
        public:
            Wall() = default;
            virtual ~Wall();


        public:
            bool Start();
            void Update();
            void Render(RenderContext& rc);


        public:
            // 物理を変化させる。
            void SwitchMode();


        // セッター。
        public:
            // 座標を設定。
            inline void SetPos(const Vector3& pos)
            {
                pos_ = pos;
            }


        // ゲッター。
        public:
            // 座標を取得。
            inline const Vector3 GetPos() const
            {
                return pos_;
            }


        private:
            // 物理を作り直す関数
            void RefreshPhysics();


        private:
            PhysicsStaticObject* pPhysics_ = nullptr;
            Player* pPlayer_ = nullptr;
            app::ui::ButtonActionUI* pDrawTiming_ = nullptr;


        private:    
            Vector3 pos_ = Vector3::Zero;
            Vector3 scale_3D_Normal = Vector3(0.1f, 0.1f, 0.1f);
            Vector3 scale_2D_Wide = Vector3(0.1f, 0.1f, 2.0f);
            Vector3 currentScale_ = Vector3::One;


            ModelRender render_;
            Quaternion rot_ = Quaternion::Identity;
            bool is2DMode_ = false;

            float kShowDistance_ = 80.0f;
        };
    } 
} 
