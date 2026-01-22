#pragma once
#include "Src/Actor/Stage/Gimmick/IGimmic.h"

class Player;

namespace app
{
    namespace gimmick
    {
        class FireBall : public IGimmic
        {
        public:
            FireBall() = default;
            virtual ~FireBall();


        public:
            bool Start()override;
            void Update() override;
            // エフェクトを用いるため、今回は空実装。
            void Render(RenderContext& rc) override {};


        public:
            // パラメータ設定。
            void SetParameter(const Vector3& startPos, const Vector3& dir, float speed);


        private:
            // エフェクトの位置を更新する。
            void UpdateEffectPosition();

            // Playerとの当たり判定を取る。
            void CheckPlayerCollision();

            // エフェクトを生成・初期化。
            void CreateEffect();

        private:
            Player* pPlayer_ = nullptr;
            EffectEmitter* pEffectEmitter_ = nullptr;

        private:
            // エフェクト本体を管理する変数。
            int effectHandle_ = -1;


            // 移動用変数。
            Vector3 position_ = Vector3::Zero;
            Vector3 direction_ = Vector3::Zero;
            float speed_ = 0.0f;


            // 画面外に飛び出さな用に寿命を管理する変数。
            float lifeTime_ = 0.0f;

            // 設定用の変数は定数のため.cppに。
            // 当たり判定の半径をも同様。


            Quaternion rotation_ = Quaternion::Identity;
        };

    }
}

