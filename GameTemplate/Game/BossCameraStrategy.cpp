// Src/Camera/BossCameraStrategy.cpp

#include "stdafx.h"

#include "Src/Actor/Character/Enemy/Boss.h"
#include "Src/Actor/Character/Player.h"
#include "BossCameraStrategy.h"
#include "Src/Camera/SideCameraStrategy.h"
#include "Src/Camera/FollowStrategy.h"

namespace
{
    const float TARGET_HEIGHT = 150.0f;
    const float ORIGINCAMERA_EVENTTIME = 4.0f; // 登場カメラシーンの時間

    const float NOW_XPOS = -200.0f; // X座標を動かす量 (登場時のイージング量)

    const float CAMERAMOVESPEED_X = 200.0f; // カメラのX座標移動速度。
    const float SWITCH_SPEED = 10.0f;       // 視点を切り替える速度

    const Vector3 ORIGINCAMERA_BOSSPOS(0.0f, 200.0f, -400.0f); // ボス戦時の登場カメラの原点位置。
    const Vector3 SIDE_VIEW_OFFSET = {-500.0f, 150.0f, 0.0f};  // ボスの側面から
    const Vector3 DEPTH_VIEW_OFFSET = {0.0f, 150.0f, -500.0f};
} // namespace

// --- 初期化処理の改善 ---
bool BossCameraStrategy::Start()
{
    // ボスオブジェクトを検索し、ポインタを初期化
    pBoss_ = FindGO<Boss>("boss");
    if (!pBoss_)
    {
        // ボスが見つからなくても、続行は可能だがログを出しておく
        K2_LOG("BossCameraStrategy::Start() Bossが見つかりません。\n");
        // ただし、イベントカメラや戦闘カメラは機能しない
    }

    // 状態を初期状態（登場）にリセット
    currenState_ = BossCameraState::Appearance;
    eventTimerLapse_ = 0.0f;
    currentViewMode_ = BattleViewMode::SidwView;

    return true;
}

// --- Update処理の改善 (状態遷移に基づく) ---
void BossCameraStrategy::Update(nsK2EngineLow::Camera* pCamera, const float deltaTime)
{
    // ボスが見つからない場合は処理をスキップ
    if (!pBoss_)
        return;

    switch (currenState_)
    {
    case BossCameraState::Appearance:
        BossAppearanceCamera(); // 登場イベントカメラ
        break;

    case BossCameraState::Battle:
        BattleCamera(pCamera, deltaTime); // 戦闘中カメラ
        break;
    }

    // カメラの視点や向きを変更する処理
    BossCameraGetState(pCamera, deltaTime);
}

void BossCameraStrategy::BossAppearanceCamera()
{
    // ボス登場時のカメラ移動・演出処理
    if (!pBoss_)
        return; // Startで検索済みのチェック

    // カメラの経過時間を計算。
    eventTimerLapse_ += g_gameTime->GetFrameDeltaTime();

    // ボスの位置を注視点とする
    Vector3 bossPos = pBoss_->GetPos();
    Vector3 cameraTarget = bossPos;
    cameraTarget.y += TARGET_HEIGHT;
    g_camera3D->SetTarget(cameraTarget);

    // カメラ位置の初期設定 (ORIGINCAMERA_BOSSPOSをベースにボス位置からオフセット)
    Vector3 originPos = bossPos + ORIGINCAMERA_BOSSPOS;
    g_camera3D->SetPosition(originPos);

    // X座標をイージングする。（登場時の演出）
    float elapseRate = eventTimerLapse_ / ORIGINCAMERA_EVENTTIME;
    float nowXpos = NOW_XPOS * elapseRate;
    cameraTarget.x += nowXpos;

    // イベントカメラの終了判定。
    if (eventTimerLapse_ >= ORIGINCAMERA_EVENTTIME)
    {
        // 経過時間リセット。
        eventTimerLapse_ = 0.0f;

        // 状態を戦闘中に切り替え
        currenState_ = BossCameraState::Battle;
    }
}

void BossCameraStrategy::BattleCamera(nsK2EngineLow::Camera* pCamera, const float deltaTime)
{
    // ボス戦闘中のカメラ追従・切り替え処理
    if (!pBoss_)
        return;

    Vector3 bossPos = pBoss_->GetPos();

    // 1. 理想のカメラ位置 (Ideal Position) を設定
    Vector3 targetOffset = Vector3::Zero;
    switch (currentViewMode_)
    {
    case BattleViewMode::SidwView:
        // 側面視点: ボスに対してX軸にオフセット
        targetOffset = SIDE_VIEW_OFFSET;
        break;

    case BattleViewMode::DepthView:
        // 奥行き視点: ボスに対してZ軸にオフセット
        targetOffset = DEPTH_VIEW_OFFSET;
        break;
    }

    Vector3 idealPos = bossPos + targetOffset;

    // 2. 現在のカメラ位置から理想の位置へ徐々に近づける
    Vector3 currentPos = pCamera->GetPosition();
    Vector3 newPos = FollowStrategy::Lerp(SWITCH_SPEED * deltaTime, idealPos, currentPos);

    pCamera->SetPosition(newPos);

    // 3. 注視点を設定
    Vector3 target = bossPos;
    target.y += TARGET_HEIGHT;
    pCamera->SetTarget(target);
}

void BossCameraStrategy::BossCameraGetState(nsK2EngineLow::Camera* pCamera, const float deltaTime)
{
    // 追加のカメラ状態処理があればここに記述
}
