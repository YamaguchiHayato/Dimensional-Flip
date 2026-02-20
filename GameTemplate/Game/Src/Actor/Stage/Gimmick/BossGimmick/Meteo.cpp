#include "stdafx.h"
#include "Src/Actor/Stage/Gimmick/BossGimmick/Meteo.h"

namespace
{
    const auto WARNING_TIME = 2.0f;
}


namespace app
{
    namespace gimmick
    {
        bool Meteo::Start()
        {
            const std::string meteoPath = InitGimmick("StageEX/FallingRocks");
            render_.Init(meteoPath.c_str());



            // 初期位置をセット。
            render_.SetPosition(currentPos_);

            // マーカーを生成する。
            pMarker_ = NewGO<app::production::AttackMarker>(0);
            pMarker_->SetTransform(targetPos_, Vector3(0.5f, 0.5f, 0.5f));
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

                lifeTime_ += g_gameTime->GetFrameDeltaTime();

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


            default:
                break;
            }

        }
         

        void Meteo::Render(RenderContext& rc)
        {
            render_.Draw(rc);
        }
    }
}
