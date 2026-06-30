#include "stdafx.h"
#include "Src/Actor/Stage/Gimmick/BossGimmick/Meteo.h"
#include "Src/Actor/Character/Player/Player.h"

namespace
{
    // 警告時間。
    const auto WARNING_TIME = 2.0f;

    // 当たり判定の半径。
    const auto HIT_RADIUS = 15.0f;
}


namespace app
{
    namespace gimmick
    {
        bool Meteo::Start()
        {
            // モデル名をセット。
            const std::string meteoPath = InitGimmick("StageEX/FallingRocks");
            render_.Init(meteoPath.c_str());

            // Playerを探索。
            pPlayer_ = FindGO<Player>("player");
            hasHit = false;

            // 初期位置をセット。
            render_.SetPosition(currentPos_);

            // マーカーを生成する。
            pMarker_ = NewGO<AttackMarker>(0);
            pMarker_->SetTransform(targetPos_, Vector3::One * 0.25);

            // 予兆時間をセットする。
            pMarker_->SetDuration(warningTime_);

            return true;
        }


        void Meteo::Update()
        {
            // 状態を更新する。
            UpdateState();

            // スケールをセットする。
            render_.SetScale(Vector3(0.05f, 0.05f, 0.05f));
            // 座標を更新する。
            render_.SetPosition(currentPos_);
            // オブジェクト本体を更新する。
            render_.Update();
        }


        void Meteo::UpdateState()
        {
            switch (state_)
            {
            case State::Warning:
            {
                // 経過時間を加算する。
                timer_ += g_gameTime->GetFrameDeltaTime();

                if (timer_ >= warningTime_)
                {
                    // 落下ステートへ移行する。
                    state_ = State::Falling;

                    // 動き始めるとマーカーを削除する。
                    if (pMarker_)
                    {
                        DeleteGO(pMarker_);
                        pMarker_ = nullptr;
                    }
                }
                break;
            }


            case State::Falling:
            {
                // 落下処理。
                currentPos_.y -= fallSpeed_;

                // 当たっていなければ当たり判定を行う。
                if (!hasHit)
                    if (CheckCollision())
                        hasHit = true;

                // 削除までの時間を加算する。
                lifeTime_ += g_gameTime->GetFrameDeltaTime();

                // 一定時間経過するか、地面に到達したら削除する。
                if (lifeTime_ > 10.0f || currentPos_.y < -300.0f)
                {
                    if (pMarker_)
                    {
                        DeleteGO(pMarker_);
                        pMarker_ = nullptr;
                    }

                    DeleteGO(this);
                }
                break;
            }
            }

        }


        bool Meteo::CheckCollision()
        {
            // 早期リターン。
            if (!pPlayer_)
                return false;

            // プレイヤーの座標を取得する。
            Vector3 playerPos = pPlayer_->GetPlayerPos();
            playerPos.y += 20.0f;

            // 距離判定。
            Vector3 diff = currentPos_ - playerPos;
            if (diff.LengthSq() <= HIT_RADIUS * HIT_RADIUS)
            {
                // ダメージ処理。
                pPlayer_->OnDamage(1);
                hasHit = true;

                DeleteGO(this);
                return true;
            }

            return false;
        }


        void Meteo::Render(RenderContext& rc)
        {
            render_.Draw(rc);
        }
    }
}
