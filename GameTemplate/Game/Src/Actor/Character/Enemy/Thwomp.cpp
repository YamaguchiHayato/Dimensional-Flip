#include "stdafx.h"
#include "Thwomp.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Core/SoundManager.h"
#include "Src/Core/StageManager.h"
#include <algorithm>


namespace
{
    const Vector3 SCALE    (0.125f, 0.125f, 0.125f);
    const Vector3 BOX_SIZE(2.0f, 2.0f, 2.0f);


    auto RADIUS = 8.0f;

    struct MoveStatus
    {
        static constexpr auto INITIAL_VELOCITY = 20.0f;     // 初速。
        static constexpr auto MAXIMUM_SPEED = 25.0f;        // 最大速度。
        static constexpr auto GRAVITY = 1000.0f;            // 重力加速度。
        static constexpr auto TERMINAL_VELOCITY = -1000.0f; // 落下速度を緩和させる。
        static constexpr auto ROTATION_SCALE = 5.0f;        // 回転速度倍率。
    };

}


namespace app
{
    namespace enemy
    {
        Thwomp::~Thwomp()
        {
            if (pCollisionObject_ != nullptr)
            {
                DeleteGO(pCollisionObject_);
                pCollisionObject_ = nullptr;
            }
        }


        bool Thwomp::Start()
        {
            // 1. モデル初期化
            std::string enemyPath = InitModel("Stage2/Enemy/RotationEnemy");
            render_.Init(enemyPath.c_str());

            initPos_ = pos_;

            render_.SetPosition(pos_);
            render_.SetRotation(rot_);
            render_.SetScale(SCALE);
            render_.Update();

            if (moveDir_.LengthSq() > 0.001f)
            {
                Vector3 dir = moveDir_;
                dir.Normalize();

                float yaw = atan2f(dir.x, dir.z);
                Quaternion qYaw;
                qYaw.SetRotation(Vector3::AxisY, yaw);

                float pitch = -asinf(dir.y);
                Quaternion qPitch;
                qPitch.SetRotation(Vector3::AxisX, pitch);

                slopeRot_ = qYaw * qPitch;
            }

            else
                slopeRot_ = Quaternion::Identity;

		    pPlayer_ = FindGO<Player>("player");

            // サイズは敵の大きさに合わせて調整 (半径, 高さ, 座標)
            charaCon_.Init(2.0f, 3.0f, pos_);
            // ユーザーインデックスの設定（敵キャラとして認識させるため）
            // 敵の属性値を0に設定。
            auto* myBody = charaCon_.GetRigidBody()->GetBody();
            auto* playerBody = pPlayer_->GetCharacterController().GetRigidBody()->GetBody();

			// Playerと 敵 が衝突時のみ計算を行わないように設定。
            myBody->setIgnoreCollisionCheck(playerBody, true);

            // コリジョンの初期化
            pCollisionObject_ = NewGO<CollisionObject>(0, "thwomp_collision");
            pCollisionObject_->CreateBox(pos_, slopeRot_, BOX_SIZE);
            pCollisionObject_->SetIsEnableAutoDelete(false);

            pos_ = initPos_;
            RADIUS = BOX_SIZE.x * 0.5f;
            isRespawning_ = false;

            // 半径の設定。
            RADIUS = 8.0f;
            // 速度初期化
            velocity_ = Vector3::Zero;
            // ステートの初期化。
            moveState_ = MoveState::state_Falling;
            return true;
        }

        
        void Thwomp::Update()
        {
            // 経過時間の取得。
            const float deltaTime = g_gameTime->GetFrameDeltaTime();


            // プレイヤーの座標をチェック。
			if (!isTriggered_)
			{
                if (pPlayer_->GetPlayerPos().x >= 1000.0f)
                    isTriggered_ = true;

                return;
			}

            // 超えているならタイマーを開始させる。
            if (!isActive_)
            {
                delayTimer_ += deltaTime;
                if (delayTimer_ >= spawnDelay_)
                    isActive_ = true;
                return;
            }

            // 回転の更新・移動処理
            SwitchMoveState();


            // 削除判定。
            if (pos_.x <= triggerPos_.x - 1000.0f || pos_.y < -1500.0f)
            {
                if (auto* pStageMgr = nsApp::nsStage::StageManager::GetInstance())
                    pStageMgr->UnregisterSpawnedObject(this);

                DeleteGO(this);
                return;
            }

            // モデルの更新。
            render_.SetPosition(pos_);
            render_.SetRotation(rot_);
            render_.Update();

            // コリジョンの更新。
            if (pCollisionObject_)
            {
                pCollisionObject_->SetPosition(pos_);
                pCollisionObject_->SetRotation(slopeRot_);
            }

        }


        void Thwomp::SwitchMoveState()
        {
            const float deltaTime = g_gameTime->GetFrameDeltaTime();
            auto* playerBody = pPlayer_->GetCharacterController().GetRigidBody()->GetBody();

			// 
            uint8_t currentIndex = playerBody->getUserIndex();
            // playerの属性値を0に。
            playerBody->setUserIndex(0); 



            switch (moveState_)
            {
            case MoveState::state_Falling:
            {
                // 落下中のみ着地判定を持つ。
                // y軸に重力を書けて垂直落下を行う。
                velocity_.y -= MoveStatus::GRAVITY * deltaTime;

                // 移動の実行。
                pos_ = charaCon_.Execute(velocity_, deltaTime);

                // 着地すると
                if (charaCon_.IsOnGround())
                {
                    // 地面に着地するとSEを再生。
                    app::core::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_Fall);

                    // 次のステートに遷移。
                    moveState_ = MoveState::state_Waiting;
                    // 停止時間のリセット。
                    waitTimer_ = 0.0f;
                    // 速度のリセット。
                    velocity_ = Vector3::Zero;
                }
                break;
            }

            case MoveState::state_Waiting:
            {
                // モデルを停止(落下後の余韻)。
                waitTimer_ += deltaTime;

                // 
                if (waitTimer_ >= stopTimer_)
                {
                    app::core::SoundManager::GetInstance()->PlaySE(GameSoundList_SE_Rotation, true);
                    moveState_ = MoveState::state_Moving;
                }
                break;
            }

            case MoveState::state_Moving:
            {
                // 移動ロジックの実行。
                Move(deltaTime);
                break;
            }



            // モデルの更新。
            render_.SetPosition(pos_);
            render_.SetRotation(rot_);
            render_.Update();

            // コリジョンの更新。
            if (pCollisionObject_)
            {
                pCollisionObject_->SetPosition(pos_);
                pCollisionObject_->SetRotation(slopeRot_);
            }
            }

            // playerの属性値を元に戻す。
            playerBody->setUserIndex(currentIndex);

        }


        void Thwomp::Render(RenderContext& rc)
        {
            render_.Draw(rc);
        }


        void Thwomp::Move(float deltaTime)
        {
            Vector3 dir = moveDir_;
            if (dir.LengthSq() < 1e-6f)
                return;

            const bool onGround = charaCon_.IsOnGround();

            if (onGround)
            {
                // --- 斜面加速 ---
                // 加速度を計算 (a = G * sinθ)
                float slopeAccel = MoveStatus::GRAVITY * fabsf(dir.y);
                alongSpeed_ += slopeAccel * deltaTime;

                // 最大速度の制限
                if (alongSpeed_ > MoveStatus::MAXIMUM_SPEED)
                    alongSpeed_ = MoveStatus::MAXIMUM_SPEED;

                // --- 速度ベクトルの生成 ---
                velocity_ = dir * alongSpeed_;

                // 斜面への吸着
                velocity_.y -= 2.0f;
            }
            else
            {
                // --- 空中落下 ---
                velocity_.y -= MoveStatus::GRAVITY * deltaTime;
                velocity_.x = dir.x * alongSpeed_;
                velocity_.z = dir.z * alongSpeed_;

                if (velocity_.y < MoveStatus::TERMINAL_VELOCITY)
                    velocity_.y = MoveStatus::TERMINAL_VELOCITY;
            }

            // 移動実行
            pos_ = charaCon_.Execute(velocity_, deltaTime);

            // --- 回転同期の修正 ---
            const auto distance = alongSpeed_ * deltaTime;
            if (fabsf(distance) > 1e-6f)
            {
                Vector3 up(0, 1, 0);
                Vector3 axis = Cross(up, dir);
                if (axis.LengthSq() > 1e-6f)
                {
                    axis.Normalize();
                    // 半径(125.0f)に対して正確な角度を計算
                    // ROTATION_SCALEは見た目の調整用に残す
                    const auto angle = (distance / RADIUS) * MoveStatus::ROTATION_SCALE;

                    Quaternion q;
                    q.SetRotation(axis, angle);
                    rot_ = q * rot_;
                    rot_.Normalize();
                }
            }
        }


    } 
}

