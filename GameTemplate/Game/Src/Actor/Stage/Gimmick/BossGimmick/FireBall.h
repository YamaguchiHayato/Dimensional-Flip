#pragma once
#include "Src/Actor/Stage/Gimmick/IGimmic.h"

namespace app
{
    namespace gimmick
    {
        class FireBall : public IGameObject
        {
        public:
            FireBall() = default;
            virtual ~FireBall();

        public:
            bool Start();
            void Update() ;
            // エフェクトを用いるため、今回は空実装。
            void Render(RenderContext& rc)  {};


        // セッター。
        public:
            // パラメータ設定。
            void SetParameter(const Vector3& startPos, Player* target, float speed);

            // 進行方向を指定する。
            void SetDirection(const Vector3& spawnPos, const Vector3&, float progresSpeed);

            // 大きさを設定。
            inline void SetScale(const Vector3& scale)
            {
                scale_ = scale;
            }

            // 移動速度の設定。
            inline void SetSpeed(float speed)
            {
                speed_ = speed;
            }

            // 座標を設定。
            inline void SetPosition(const Vector3& position)
            {
                position_ = position;
            }

            // 角度をセット。
            inline void SetRotation(const Quaternion& rotation)
            {
                rotation_ = rotation;
            }

            // 回転のオフセット。
            inline void SetRotationOffset(const Quaternion& offset)
            {
                rotationOffset_ = offset;
            }

            // 当たり判定の有効化・無効化。
            inline void SetCollisionEnable(bool enable)
            {
                isCollisionEnabled_ = enable;
            }


        private:
            // エフェクトの位置を更新する。
            void UpdateEffectPosition();

            // Playerとの当たり判定を取る。
            bool CheckPlayerCollision();

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
            Vector3 targetPos_ = Vector3::Zero;
            float speed_ = 0.0f;


            // 画面外に飛び出さな用に寿命を管理する変数。
            float lifeTime_ = 0.0f;

            // 設定用の変数は定数のため.cppに。
            // 当たり判定の半径をも同様。

            Vector3 scale_ = Vector3::One;
            Quaternion rotation_ = Quaternion::Identity;
            Quaternion rotationOffset_ = Quaternion::Identity;

            bool isCollisionEnabled_ = true;
        };

    }
}

