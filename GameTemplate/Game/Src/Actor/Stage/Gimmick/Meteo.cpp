#include "stdafx.h"
#include "Src/Actor/Stage/Gimmick/Meteo.h"
#include "graphics/effect/EffectEmitter.h"

namespace
{
    const auto WARNING_TIME = 2.0f;

    const auto SPAWN_HEIGHT = 1200; // 出現する高さ
}


namespace app
{
    namespace gimmick
    {
        bool Meteo::Start()
        {
            const std::string meteoPath = InitGimmick("StageEX/FallingRocks");
            render_.Init(meteoPath.c_str());

            currentPos_ = targetPos_;
            currentPos_.y += SPAWN_HEIGHT;

            // 初期位置をセット。
            render_.SetPosition(currentPos_);

            return true;
        }


        void Meteo::Update()
        {
            // 状態を更新する。
            UpdateState();

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

                // 経過時間を加算する。
                timer_ += g_gameTime->GetFrameDeltaTime();

                if (timer_ >= warningTime_)
                    // 落下ステートへ移行する。
                    state_ = State::Falling;
                break;

            case State::Falling:

                // 落下処理。
                currentPos_.y -= fallSpeed_;

                // 目標の座標に達したらオブジェクトを削除する。
                if (currentPos_.y < -300.0f)
                    DeleteGO(this);

                break;

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
