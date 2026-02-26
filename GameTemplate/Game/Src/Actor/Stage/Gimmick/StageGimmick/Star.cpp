#include "stdafx.h"

#include "Star.h"



#include "Src/Core/SceneManager.h"
#include "Src/Core/SoundManager.h"

#include "Src/Production/Fade.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Production/StarGetProduction.h"


#include "Src/Production/CutIn/CutInView.h"
#include "Src/Production/GameClear.h"
#include "Src/Production/StageClear.h"

namespace
{
    const auto ROTATION_SPEED = 720.0f; //
}

namespace StarStatus
{
    const float STAR_ROTATE_SPEED = 3.0f;
    const Vector3 STAR_SCALE(0.03f, 0.03f, 0.03f);
    const Vector3 COLLISION_HEIGHT(0.0f, 25.0f, 0.0f);  // コリジョンの高さ
    const Vector3 COLLISION_SIZE(365.0f, 5.0f, 225.0f); // コリジョンの大きさ
} 


bool Star::Start()
{
    m_starRender.Init("Assets/stage/Stage1/Star.tkm");
    m_starRender.SetScale(StarStatus::STAR_SCALE);
    m_starRender.SetPosition(m_starPosition);

    m_player = FindGO<Player>("player");
    m_isGoal = false;
    return true;
}


void Star::Update()
{
    if (m_player == nullptr)
        return;

    if (m_isGoal)
    {
        return;
    }

    Vector3 diff = m_player->GetPlayerPos() - m_starPosition;
    if (diff.Length() > 5.0f)
    {
        Rotation(); // 通常のゆっくりした回転
        m_starRender.SetPosition(m_starPosition);
        m_starRender.Update();
        return;
    }

    m_isGoal = true;
    disPlayTimer_ = 0.0f;
    displayDuration_ = 0.8f; // 演出の長さ（秒）

    auto* pProduction = NewGO<app::production::StarGetProduction>(0);
    pProduction->StartSequence(m_player, this);
}

void Star::Rotation()
{
    m_starRotation.AddRotationDegY(StarStatus::STAR_ROTATE_SPEED);

    m_starRender.SetRotation(m_starRotation);
}


void Star::Render(RenderContext& rc)
{
    m_starRender.Draw(rc);
}


void Star::UpdateMovement(float deltaTime)
{
    // タイマー更新と進捗(0.0～1.0)の計算
    disPlayTimer_ += deltaTime;
    auto progress = disPlayTimer_ / displayDuration_;

    // ガード節: 演出終了判定
    if (progress > 1.0f)
    {
        return;
    }

    // --- 演出ロジック ---
    auto rotationSpeed = StarStatus::STAR_ROTATE_SPEED * (1.0f + progress * 10.0f);
    m_starRotation.AddRotationDegY(rotationSpeed);

    auto risePower = powf(progress, 3.0f) * 150.0f;
    Vector3 drawPos = m_starPosition;
    drawPos.y += risePower; // 元の座標に上昇分をプラス

    auto sdaleFactor = (progress < 0.2f) ? (1.0f + progress * 2.0f) : (1.0f - (progress - 0.2f) / 0.8f);

    // --- 重要：計算した値をレンダラーに反映 ---
    m_starRender.SetScale(StarStatus::STAR_SCALE * sdaleFactor);
    m_starRender.SetPosition(drawPos);        // 上昇した座標を使う
    m_starRender.SetRotation(m_starRotation); // 更新した回転を使う
    m_starRender.Update();
}


void Star::OnEffectFinished()
{
    // フェードを取得。
    Fade* pFade = SceneManager::GetInstance()->GetFade();
    if (pFade)
        pFade->StartFadeIn();

    // StageClearの生成。
    NewGO<StageClear>(0, "");
    DeleteGO(this);
}

