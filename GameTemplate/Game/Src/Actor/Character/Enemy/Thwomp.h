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


        public:
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

            // 時間を設定する。
            inline void SetSpawnDelay(float delay)
            {
                spawnDelay_ = delay;
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

        private:    
            ModelRender render_;

            CharacterController charaCon_;
            Quaternion slopeRot_ = Quaternion::Identity; // コリジョン用の固定角度
            Quaternion rot_ = Quaternion::Identity;      // モデル用の回転
            Quaternion baseRot_ = Quaternion::Identity;  // 初期回転

            Vector3 pos_ = Vector3::Zero;                // 現在位置
            Vector3 initPos_ = Vector3::Zero;            // 初期位置
            Vector3 moveDir_ = Vector3::Zero;            // 進行方向
            Vector3 velocity_ = Vector3::Zero;
            Vector3 triggerPos_ = Vector3::Zero;
            Vector3 lastGroundNormal_ = Vector3(0, 1, 0);

            // パラメータ
            float moveSpeed_ = 20.0f; // Z軸方向の移動スピード

            float alongSpeed_ = 0.0f;
            float maxAlongSpeed_ = 2500.0f; // 加速し続けない上限
            float groundStick_ = 20.0f;     // 接地安定用の下向き押し付け
            float rollAngle_ = 0.0f;        // 見た目の回転角度
            float slopeEnterY_ = 0.0f;      // 斜面に入ったときのY座標
            bool isActive_ = false;
            bool isRespawning_ = false;


        // モデルの動きを管理する変数群。
        private:
            // モデルの移動状態を管理する列挙。
            enum class MoveState : uint8_t
            {
                state_Falling, // 落下中。
                state_Waiting, // 待機中。
                state_Moving,  // 移動中。
                state_Num,     // 状態数。
            };

            // 現在の移動状態。
            MoveState moveState_ = MoveState::state_Num;

            // タイマー。
            float waitTimer_ = 0.0f; // 待機時間計測用タイマー。
            float stopTimer_ = 1.5f; // 待機時間。

            // SEを鳴らしたかどうか。
            bool hasLandedSE_ = false; // 着地時のSE再生フラグ。

            uint8_t rollingSEHandle_ = -1; // 回転SEの管理用
        private:
            // MoveStateの切り替え処理。
            void SwitchMoveState();



        // 時間差で再出現させるための変数群。
        private:    
            float spawnDelay_ = 0.0f;  // 再出現までの遅延時間
            float delayTimer_ = 0.0f;  // 
            bool isTriggered_ = false; // 


        };

    }
}

