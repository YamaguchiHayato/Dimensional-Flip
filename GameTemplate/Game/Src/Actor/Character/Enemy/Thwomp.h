#pragma once
#include "Src/Actor/Character/Enemy/IEnemy.h"

class Player;

/// <summary>
/// Z軸方向に転がりながら移動し、重力で落下するトゲ敵。
/// CharacterControllerを使用して移動制御を行う。
/// </summary>

namespace app
{
    namespace enemy
    {
        class Thwomp : public app::enemy::IEnemy
        {
        public:
            Thwomp() = default;
            virtual ~Thwomp();

            bool Start() override;
            void Update() override;
            void Render(RenderContext& rc) override;

        public:
            // トリガー座標をセット。
            inline void SetTriggerPos(const Vector3& pos)
            {
                triggerPos_ = pos;
            }


            // 座標をセット。
            inline void SetPos(const Vector3& pos) override
            {
                InitPos(pos);
            }


            // 大きさをセット。
            inline void SetScale(const Vector3& scale) override{};


            // 踏みつけて倒せるかどうかをセット。
            inline void SetStompable(bool enable) override{};


            // 初期位置の設定
            inline void InitPos(const Vector3& pos)
            {
                pos_ = pos;
                initPos_ = pos;
                rot_ = Quaternion::Identity;
            }


            // 移動方向設定
            inline void InitMoveDir(const Vector3& dir)
            {
                moveDir_ = dir;
                if (moveDir_.LengthSq() > 0.0f)
                    moveDir_.Normalize();
            }

        public:
            // アクティブ状態の取得
            inline bool IsActive() const { return isActive_; }

        private:
            // 移動・回転・物理挙動の更新
            void Move(float deltaTime);
            // モデルパスの生成
            inline const std::string InitModel(const std::string& gimmickname)
            {
                return "Assets/stage/" + gimmickname + ".tkm";
            }


        private:
            Player* pPlayer_ = nullptr;
            CollisionObject* pCollisionObject_ = nullptr; // 当たり判定用（ゴースト）
            ModelRender render_;

            CharacterController charaCon_;
            Quaternion slopeRot_ = Quaternion::Identity; // コリジョン用の固定角度
            Quaternion rot_ = Quaternion::Identity;      // モデル用の回転
            Vector3 pos_ = Vector3::Zero;                // 現在位置
            Vector3 initPos_ = Vector3::Zero;            // 初期位置
            Vector3 moveDir_ = Vector3::Zero;            // 進行方向
            Vector3 velocity_ = Vector3::Zero;
            Vector3 triggerPos_ = Vector3::Zero;

            // パラメータ
            float moveSpeed_ = 200.0f; // Z軸方向の移動スピード
            float radius_ = 125.0f;    // 回転半径

            const float GRAVITY = 15.0f; // 重力加速度

            bool isActive_ = false;
            bool isRespawning_ = false;
        };

    }
}

