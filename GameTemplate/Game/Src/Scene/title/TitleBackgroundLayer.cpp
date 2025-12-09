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
    const float SPEED = 1.5f; // 1秒間にn(設定値)秒。

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
    float angle = -SPEED * g_gameTime->GetFrameDeltaTime();

    // Y軸回転。
    Quaternion deltaRot = Quaternion::Identity;
    deltaRot.SetRotation(Vector3::AxisZ, angle);

    // 新しい回転を合成させる。
    rot_ *= deltaRot;
    // 回転ベクトルを正規化する。
    rot_.Normalize();
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
