#include "stdafx.h"

#include "Src/Actor/Stage/Gimmick/BossGimmick/Spear.h"
#include "Src/Actor/Character/Player/Player.h"

namespace
{
    const auto WARNING_TIME = 1.5f; // 予兆時間 (震えている時間)

    // 垂直用
    const auto RISE_SPEED = 7.0f;
    const auto RETRACT_SPEED = 20.0f;
    const auto SPAWN_DEPTH = -300.0f;

    const auto CROSS_SPEED = 180.0f;

    const auto WAIT_TIME = 1.0f;

    const auto HIT_RADIUS = 20.0f;
} // namespace

namespace app
{
    namespace gimmick
    {
        bool Spear::Start()
        {
            const std::string path = InitGimmick("StageEX/spear");
            render_.Init(path.c_str());
            render_.SetScale(Vector3(0.5f, 0.5f, 0.5f));

            // Playerクラスを探索処理。
            pPlayer_ = FindGO<Player>("player");
            hasHit_ = false;

            if (type_ == SpearType::Cross)
            {
                currentPos_ = startPos_;
                render_.SetPosition(currentPos_);

                // 進行方向の計算
                Vector3 toTarget = targetPos_ - startPos_;
                if (toTarget.LengthSq() > 0.001f)
                {
                    moveDir_ = toTarget;  // コピー
                    moveDir_.Normalize(); // 正規化
                }
                else
                {
                    moveDir_ = Vector3::Front;
                }

                // ★回転: 進行方向に穂先(Y軸)を向ける
                Quaternion rot;
                rot.SetRotation(Vector3::AxisY, moveDir_);

                // バラつき演出用のロール回転
                float randomRoll = static_cast<float>(rand() % 360);
                Quaternion rollRot;
                rollRot.SetRotationDegY(randomRoll);

                render_.SetRotation(rot * rollRot);
            }
            else
            {
                // 垂直 (既存のまま)
                currentPos_ = targetPos_;
                currentPos_.y += SPAWN_DEPTH;
                render_.SetPosition(currentPos_);

                pMarker_ = NewGO<app::production::AttackMarker>(0);
                pMarker_->SetTransform(targetPos_, Vector3(0.5f, 0.5f, 0.5f));
                pMarker_->SetDuration(WARNING_TIME);
            }

            return true;
        }


        void Spear::Update()
        {
            lifeTime_ += g_gameTime->GetFrameDeltaTime();
            if (lifeTime_ > 10.0f)
            {
                DeleteGO(this);
                return;
            }

            // 攻撃中かつ、まだ当たっていない場合、判定を行う。
            if (state_ == SpearState::Rising && !hasHit_)
            {
                CheckCollision();
            }


            UpdateState();

            render_.SetPosition(currentPos_);
            render_.Update();
        }


        void Spear::UpdateState()
        {
            auto deltaTime = g_gameTime->GetFrameDeltaTime();

            switch (state_)
            {
            case SpearState::Idle:
                timer_ += deltaTime;

                if (type_ == SpearType::Cross)
                {
                    float power = (timer_ / WARNING_TIME) * 1.5f; // 時間経過で激しく
                    Vector3 shake;
                    shake.x = (static_cast<float>(rand() % 100) / 50.0f - 1.0f) * power;
                    shake.y = (static_cast<float>(rand() % 100) / 50.0f - 1.0f) * power;
                    shake.z = (static_cast<float>(rand() % 100) / 50.0f - 1.0f) * power;
                    currentPos_ = startPos_ + shake; // startPos_を基準に揺らす
                }

                if (timer_ >= WARNING_TIME)
                {
                    state_ = SpearState::Rising;
                    timer_ = 0.0f;
                    if (pMarker_)
                    {
                        DeleteGO(pMarker_);
                        pMarker_ = nullptr;
                    }
                }
                break;


            case SpearState::Rising:
                if (type_ == SpearType::Cross)
                {
                    // ★目標に向かって高速移動
                    currentPos_ += moveDir_ * CROSS_SPEED * deltaTime;

                    Vector3 toTargetVec = targetPos_ - startPos_;
                    Vector3 toCurrentVec = currentPos_ - startPos_;

                    // 現在の移動距離が、目標までの距離を超えたら停止
                    if (toCurrentVec.LengthSq() >= toTargetVec.LengthSq())
                    {
                        currentPos_ = targetPos_; // ピタッと止める
                        state_ = SpearState::Wait;
                        timer_ = 0.0f;
                    }
                }
                else
                {
                    // 垂直移動
                    currentPos_.y += RISE_SPEED;
                    float reachHeight = targetPos_.y + 300.0f;
                    if (currentPos_.y >= reachHeight)
                    {
                        currentPos_.y = reachHeight;
                        state_ = SpearState::Wait;
                        timer_ = 0.0f;
                    }
                }
                break;


            case SpearState::Wait:
                timer_ += deltaTime;
                if (timer_ >= WAIT_TIME)
                    state_ = SpearState::Retract;
                break;


            case SpearState::Retract:
                if (type_ == SpearType::Cross)
                {
                    // そのまま消すか、フェードアウトさせる
                    DeleteGO(this);
                }
                else
                {
                    // 垂直は戻る
                    currentPos_.y -= RETRACT_SPEED;
                    if (currentPos_.y < SPAWN_DEPTH)
                        DeleteGO(this);
                }
                break;
            }
        }


        void Spear::CheckCollision()
        {
            if (!pPlayer_)
                return;

            // プレイヤーの座標を取得する。
            Vector3 playerPosition = pPlayer_->GetPlayerPos();
            playerPosition.y += 5.0f;


            // 距離ベクトルを判定する。
            Vector3 diff = playerPosition - currentPos_;
            if (diff.LengthSq() < HIT_RADIUS * HIT_RADIUS)
            {   
                pPlayer_->OnDamage(1);
                hasHit_ = true;
            }
        }


        void Spear::Render(RenderContext& rc)
        {
            render_.Draw(rc);
        }
    } 
} 
