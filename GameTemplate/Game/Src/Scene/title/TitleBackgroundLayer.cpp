#include "stdafx.h"
#include "Src/Scene/title/TitleBackgroundLayer.h"
#include "Src/Scene/title/TitleLayerBase.h"

namespace
{
    // 画像の解像度。
    const float WIDTH = 512.0f;
    const float HEIGHT = 512.0f;

    const float BLACK_WIDTH = 512.0f;
    const float BLACK_HEIGHT = 512.0f;



    // 回転速度。
    const float SPEED = 3.0f; // 1秒間にn(設定値)秒。

    // 回転用イージング時間。

    // 待機状態。
    const auto IDLE_SPPED = Math::PI / 16.0f; // ゆっくり回転する速度。
    const auto IDLE_DURATION = 2.0f;          // 次の回転までの待ち時間。

    // 回転前の予備動作。
    const auto PRE_FLIP_DURATION = 0.3f;      // 予備動作時間。
    const auto PRE_FLIP_AMOUNT = -0.3f;       // 逆方向に引く角度。

    // 本回転。
    const auto FLIP_DURATION = 1.2f;        ; // 180°回転にかかる時間。

    // 余韻。
    const auto POST_FLIP_DURATION = 0.3f;     // 余韻時間。
}

bool TitleBackgroundLayer::Start()
{
    // 回転用画像。
    std::string backgroundPath = Init("ground");
    titleLayerRender_.Init(backgroundPath.c_str(), WIDTH, HEIGHT);

    // 黒背景用画像。
    std::string blackPath = Init("Black");
    blackSpriteRender_.Init(blackPath.c_str(), BLACK_WIDTH, BLACK_HEIGHT);

    // 回転パラメータの初期化。
    InitRotParam();

    return true;
}


void TitleBackgroundLayer::Update()
{
    // 一定周期で回転。
    Rotation();

    // 画像を更新する。
    titleLayerRender_.SetRotation(rot_);
    titleLayerRender_.SetPosition(Vector3::Zero);
    float scale = 4.5f;
    titleLayerRender_.SetScale(Vector3(scale, scale, scale)); 
    titleLayerRender_.Update();

    // 背景黒画像の設定。
    blackSpriteRender_.SetScale(Vector3::One);
    // 黒背景画像を更新する。
    blackSpriteRender_.Update();
}


void TitleBackgroundLayer::Rotation()
{
    float dt = g_gameTime->GetFrameDeltaTime();
    stateTimer_ += dt;

    switch (state_)
    {
    // ------------------------------------------------
    // A. 待機中。
    // ------------------------------------------------
    case FlipState::Idle:
        currentAngle_ += IDLE_SPPED * dt;

        if (stateTimer_ >= IDLE_DURATION)
        {
           // 次の状態へ遷移する。
           state_ = FlipState::PreFlip;
           stateTimer_ = 0.0f;

           // 現在の角度から少し左へ戻す事を目標に設定。
           startAngle_ = currentAngle_;
           targetAngle_ = currentAngle_ + PRE_FLIP_AMOUNT;
        }
        break;

    // ------------------------------------------------
    // B. 予備動作。
    // ------------------------------------------------
    case FlipState::PreFlip:
    {
        float t = stateTimer_ / PRE_FLIP_DURATION;
        if (t >= 1.0f)
        {
            t = 1.0f;

            // 次の状態へ遷移する。
            state_ = FlipState::Flipping;
            stateTimer_ = 0.0f;

            // 元の進行方向に少し戻す。
            startAngle_ = currentAngle_;

            // ゴール地点。
            targetAngle_ = startAngle_ - PRE_FLIP_AMOUNT + Math::PI;
        }

        else
        {
            // 遷移時ない場合のみイージングを課す。
            float easeDT = EaseInBack(t);
            currentAngle_ = Math::Lerp(startAngle_, targetAngle_, easeDT);
        }
    }
    break;


    // ------------------------------------------------
    // C. 反転中: 高速で回転
    // ------------------------------------------------
    case FlipState::Flipping:
    {
        float t = stateTimer_ / FLIP_DURATION;
        if (t >= 1.0f)
        {
            t = 1.0f;

            // 目標値の角度で止める。
            currentAngle_ = targetAngle_;

            // 次の状態へ遷移する。
            state_ = FlipState::PostFlip;
            stateTimer_ = 0.0f;
        }

        else
        {
            // 三次関数を用いてイージング。
            float easedT = EaseInOutCubic(t);
            currentAngle_ = Math::Lerp(startAngle_, targetAngle_, easedT);
        }

    }
    break;


    // ------------------------------------------------
    // D. 余韻
    // ------------------------------------------------
    case FlipState::PostFlip:
        // 角度は固定する。
        currentAngle_ = targetAngle_;

        if (stateTimer_ >= POST_FLIP_DURATION)
        {
            // 初期状態へ戻す。
            state_ = FlipState::Idle;
            stateTimer_ = 0.0f;
        }
        break;
    default:
        break;
    }

    rot_.SetRotation(Vector3::AxisZ, currentAngle_);
}



void TitleBackgroundLayer::Render(RenderContext& rc)
{
    // 背景補強用
    blackSpriteRender_.Draw(rc);


    // 回転画像用
    titleLayerRender_.Draw(rc);
}


void TitleBackgroundLayer::InitRotParam()
{
    // 変数の初期化。
    state_ = FlipState::Idle;

    currentAngle_ = 0.0f;
    stateTimer_ = 0.0f;
    startAngle_ = 0.0f;
    targetAngle_ = 0.0f;
}
