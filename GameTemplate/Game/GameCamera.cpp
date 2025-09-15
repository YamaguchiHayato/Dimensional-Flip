#include "stdafx.h"
#include "GameCamera.h"
#include <algorithm>

using namespace nsK2EngineLow;

namespace
{
    const Vector3 CAMERA_POS(0.0f, 200.0f, -800.0f);
    const float CAMERA_NEAR(1.0f);
    const float CAMERA_FAR(10000.0f);
}


// -------------------------------------
// 公開設定系
// -------------------------------------
void GameCamera::SetOrbitStepDeg(float degCW)
{
    m_stepAngleRad = Math::DegToRad(degCW);
}

void GameCamera::SetOrbitDuration(float seconds)
{
    m_duration = (std::max)(0.01f, seconds);
}

// -------------------------------------
// Start: 初期化
// -------------------------------------
bool GameCamera::Start()
{
    // 状態リセット
    m_isRotating = false;
    m_rotateQueue = 0;
    m_elapsed = 0.0f;
    m_targetAngleRad = 0.0f;
    m_currentAngleRad = 0.0f;

    // 初期プレイヤー位置
    m_playerPos = QueryPlayerPos();

    // 現在のカメラからオフセットを作る（g_camera3D は GraphicsEngine 初期化時に有効化）
    if (g_camera3D) {
        // いまのカメラをプレイヤーへ向け直し（必要に応じて）
        g_camera3D->SetTarget(m_playerPos);

        // 既存位置があれば、その相対オフセットを追従オフセットとして採用
        Vector3 camPos = g_camera3D->GetPosition();
        m_followOffset = camPos - m_playerPos;

        // 初期回転オフセットも合わせる
        m_initialOffset = m_followOffset;
    }
    else {
        // フォールバック（とりあえず少し後ろ）
        m_followOffset = Vector3(0, 50, -200);
        m_initialOffset = m_followOffset;
    }

    return true;
}

// -------------------------------------
// Update / Upadte: 毎フレーム
// -------------------------------------
void GameCamera::Update()
{
    // 1) プレイヤー位置を更新
    m_playerPos = QueryPlayerPos();

     

    // 2) 入力（Bボタン）で 45° の回転ステップを要求
    if (g_pad[0]->IsTrigger(enButtonB)) {
        if (!m_isRotating) {
            BeginOrbitStepCW();   // 今すぐ開始
        }
        else {
            // 現在回転中なら、完了後にもう一段回す
            ++m_rotateQueue;
        }
    }

    // 3) 回転中なら補間を進める／そうでなければ追従のみ
    const float dt = g_gameTime->GetFrameDeltaTime();   // エンジンのΔt（秒）
    if (m_isRotating) {
        TickOrbit(dt);
    }
    else {
        // 非回転時：追従（プレイヤーが動いても既存オフセットを保つ）
        if (g_camera3D) {
            g_camera3D->SetTarget(m_playerPos);
            g_camera3D->SetPosition(m_playerPos + m_followOffset);
        }
    }
}

void GameCamera::Upadte()
{
    // 互換：誤綴り版が呼ばれても内部で本処理に回す
    Update();
}

// -------------------------------------
// 内部：プレイヤー位置の取得
// -------------------------------------
Vector3 GameCamera::QueryPlayerPos() const
{
    if (m_getPlayerPos) {
        return m_getPlayerPos();
    }
    // フォールバック：ターゲットが既にプレイヤーを向いているなら流用
    if (g_camera3D) {
        return g_camera3D->GetTarget();
    }
    return Vector3::Zero; // 最終フォールバック
}

// -------------------------------------
// 内部：45°ステップを開始
// -------------------------------------
void GameCamera::BeginOrbitStepCW()
{
    // その時点の相対オフセットを初期値としてキャプチャ
    if (g_camera3D) {
        m_initialOffset = g_camera3D->GetPosition() - m_playerPos;
        // もしゼロ長になっていたら最低限の距離を与える
        if (m_initialOffset.LengthSq() < 1e-6f) {
            m_initialOffset = Vector3(0, 50, -200);
        }
    }

    m_isRotating = true;
    m_elapsed = 0.0f;
    m_currentAngleRad = 0.0f;
    m_targetAngleRad = m_stepAngleRad; // 例：-45°（LH時計回り）
}

// -------------------------------------
// 内部：回転補間の適用
// -------------------------------------
void GameCamera::TickOrbit(float dt)
{
    if (!g_camera3D) {
        // カメラが無い場合は安全に終了
        m_isRotating = false;
        m_rotateQueue = 0;
        return;
    }

    m_elapsed += dt;
    float t = m_elapsed / m_duration;
    if (t > 1.0f) t = 1.0f;

    // イージング（スムースステップ）
    const float u = EaseSmoothStep01(t);

    // 目標角に対する現在角（初期→目標を u で補間）
    const float angleNow = m_targetAngleRad * u;

    // 初期オフセットを「Y軸まわり angleNow」回転させる
    Vector3 curOffset = m_initialOffset;
    {
        Quaternion q;
        q.SetRotationY(angleNow);
        q.Apply(curOffset);  // in-place 回転
    }

    // プレイヤー中心に配置＆注視
    const Vector3 camPos = m_playerPos + curOffset;
    g_camera3D->SetTarget(m_playerPos);
    g_camera3D->SetPosition(camPos);
    // g_camera3D->Update(); // 必要なら

    // 完了判定
    if (m_elapsed >= m_duration) {
        // 最終状態を追従オフセットとする（次のフレーム以降の通常追従用）
        m_followOffset = curOffset;

        // 状態をクリア
        m_isRotating = false;
        m_elapsed = 0.0f;
        m_currentAngleRad = 0.0f;
        m_targetAngleRad = 0.0f;

        // キューが残っていれば次の 45° を直ちに開始
        if (m_rotateQueue > 0) {
            --m_rotateQueue;
            BeginOrbitStepCW();
        }
    }
    else {
        // 進捗のメモ（必要なら差分利用用）
        m_currentAngleRad = angleNow;
    }
}
