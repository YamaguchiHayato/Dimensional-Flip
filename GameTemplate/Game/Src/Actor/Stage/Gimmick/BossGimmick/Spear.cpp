#include "stdafx.h"
#include "Src/Actor/Stage/Gimmick/Spear.h"

namespace
{
    const auto WARNING_TIME = 2.5f;   // 予兆時間
    const auto RISE_SPEED = 7.0f;    // 突き上げ速度
    const auto RETRACT_SPEED = 20.0f; // 戻る速度
    const auto WAIT_TIME = 0.5f;      // 突き上げ後の静止時間
    const auto SPAWN_DEPTH = -300.0f; // 地中の深さ
}

namespace app
{
    namespace gimmick
    {
        bool Spear::Start()
        {
            const std::string path = InitGimmick("StageEX/spear");
            render_.Init(path.c_str());

            // サイズ補正。
            render_.SetScale(Vector3(0.5f, 0.5f, 0.5f));

            // 初期座標をセットする。
            currentPos_ = targetPos_;
            // 地中に埋まるように調整する。
            currentPos_.y += SPAWN_DEPTH;

            render_.SetPosition(currentPos_);

            // マーカーの生成。
            pMarker_ = NewGO<app::production::AttackMarker>(0);
            pMarker_->SetTransform(targetPos_, Vector3(0.5f, 0.5f, 0.5f));
            pMarker_->SetDuration(WARNING_TIME);

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

            // 状態を更新する。
            UpdateState();

            render_.SetPosition(currentPos_);
            render_.Update();
        }


        void Spear::UpdateState()
        {
            switch (state_)
            {
            case SpearState::Idle:
                timer_ += g_gameTime->GetFrameDeltaTime();
                if (timer_ >= WARNING_TIME)
                {
                    state_ = SpearState::Rising;
                    timer_ = 0.0f;

                    // 槍が突き上げるとマーカーを削除。
                    if (pMarker_)
                    {
                        DeleteGO(pMarker_);
                        pMarker_ = nullptr;
                    }
                }
                break;


            case SpearState::Rising:
            {
                // 上昇
                currentPos_.y += RISE_SPEED;

                float reachHeight = targetPos_.y + 300.0f;

                // 地面(0.0f)付近まで来たら停止
                if (currentPos_.y >= reachHeight)
                {
                    currentPos_.y = reachHeight; // 行き過ぎないよう補正
                    timer_ = 0.0f;
                    state_ = SpearState::Wait;
                }
                break;

            }


            case SpearState::Wait:
                // 突き出したまま少し待機
                timer_ += g_gameTime->GetFrameDeltaTime();
                if (timer_ >= WAIT_TIME)
                {
                    state_ = SpearState::Retract;
                }
                break;

            case SpearState::Retract:
                // 下降
                currentPos_.y -= RETRACT_SPEED;

                // 元の深さまで戻ったら削除
                if (currentPos_.y < SPAWN_DEPTH)
                {
                    DeleteGO(this);
                }
                break;
            }
        }


        void Spear::Render(RenderContext& rc)
        {
            render_.Draw(rc);
        }
    }
}
