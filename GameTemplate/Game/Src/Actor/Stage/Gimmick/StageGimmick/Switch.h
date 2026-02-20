#pragma once
#include "Src/Actor/Stage/Gimmick/IGimmic.h"

namespace app
{
    namespace gimmick
    {
        class Switch : public IGimmic
        {
        public:
            Switch() = default;
            virtual ~Switch() = default;

        public:
            bool Start() override;
            void Update() override;
            void Render(RenderContext& rc) override;

        public:
            void SetPos(const Vector3& pos) { gimmickPos_ = pos; }

            // 初期化処理。
            void Initialize();

            // Playerとの衝突判定。
            void OnCollisionEnter(CollisionObject* object);

            // スイッチを押す処理。
            void Press();

            // 外部（Stage1など）から「今、押されたか？」を取得する関数
            // 呼ぶとフラグが false に戻るので、1回だけ true が返ります。
            bool GetIsPressedTrigger();

        private:
            // スイッチが押されている状態か？
            bool isPressed_ = false;

            // 外部に通知するためのトリガーフラグ
            bool isTriggered_ = false;

            // 演出用：元のスケール
            Vector3 originalScale_ = Vector3::One;
        };
    } // namespace gimmick
} // namespace app
