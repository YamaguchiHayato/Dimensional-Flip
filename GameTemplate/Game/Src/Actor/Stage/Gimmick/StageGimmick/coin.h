#pragma once
#include "Src/Actor/Stage/Gimmick/IGimmic.h"

namespace app
{
    namespace gimmick
    {
        class Coin : public IGimmic
        {
        public:
            Coin() = default;
            virtual ~Coin() = default;

            bool Start() override;
            void Update() override;
            void Render(RenderContext& rc) override;

            // Playerとの衝突判定
            void OnCollisionEnter(CollisionObject* object);

            // 出現させる関数
            void Appear();

            // 取得された瞬間かどうかを確認する関数
            bool GetIsCollectedTrigger();

            void SetPos(const Vector3& pos) { gimmickPos_ = pos; }

        private:
            bool isVisible_ = false;   // 出現しているか
            bool isCollected_ = false; // 既に取られたか
            bool getTrigger_ = false;  // 取得通知用フラグ

            float rotateAngle_ = 0.0f; // 回転演出用
        };
    } // namespace gimmick
} // namespace app
