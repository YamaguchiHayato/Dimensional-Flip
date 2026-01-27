#pragma once
#include "Src/Actor/Stage/Gimmick/IGimmic.h"

// PhysicsStaticObjectが宣言されているヘッダーをインクルードできない場合は
// 前方宣言または適切なヘッダーパスを追加してください
// #include "Src/Physics/PhysicsStaticObject.h"

namespace app
{
    namespace gimmick
    {
        class FloatingPlatform : public IGimmic
        {
        public:
            FloatingPlatform() = default;
            virtual ~FloatingPlatform();


        public:
            bool Start() override;
            void Update() override;
            void Render(RenderContext& rc) override;
            inline const std::string InitGimmick(const std::string& gimmickname) override
            {
                return IGimmic::InitGimmick(gimmickname);
            };


        public:
            // 足場を出現させる。
            void Activate(const Vector3& pos, const Vector3& scale);

            // 足場を隠す。(OFF)
            void Deactivate();


        // セッター。
        public:
            // 座標の更新。
            inline void SetPos(const Vector3& pos)
            {
                targetPos_ = pos;
            }

            // 寿命の設定。
            inline void SetLifeTime(float time)
            {
                lifeTime_ = time;
            }

            // 目標位置の設定。
            inline void SetTargetPos(const Vector3& pos)
            {
                targetPos_ = pos;
            }


        private:
            // 物理判定を作り直す関数 
            void RefreshPhysics();


        private:
            // コリジョンではなく、物理オブジェクトを使用
            PhysicsStaticObject* pPhysics_ = nullptr;


        private:
            Vector3 targetPos_ = Vector3::Zero;  // 最終的な固定位置
            Vector3 currentPos_ = Vector3::Zero; // 現在の位置
            Vector3 currentScale_ = Vector3(0.1f, 0.1f, 0.1f); // 現在のスケール


            bool isActive_ = false; // 足場がアクティブかどうか

            float timer_ = 0.0f;
            float lifeTime_ = 10.0f;
        };
    }
} 
