#include "stdafx.h"

#include "Star.h"



#include "Src/Core/SceneManager.h"
#include "Src/Direction/Fade.h"
#include "Src/Actor/Character/Player/Player.h"

#include "Src/Direction/CutIn/CutInView.h"
#include "Src/Direction/GameClear.h"
#include "Src/Direction/StageClear.h"

namespace StarStatus
{
    const float STAR_ROTATE_SPEED = 3.0f;
    const Vector3 STAR_SCALE(0.03f, 0.03f, 0.03f);
    const Vector3 COLLISION_HEIGHT(0.0f, 25.0f, 0.0f);  // コリジョンの高さ
    const Vector3 COLLISION_SIZE(365.0f, 5.0f, 225.0f); // コリジョンの大きさ
} // namespace StarStatus

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
    {
        return;
    }
    if (m_isGoal)
    {
        Rotation();
        m_starRender.Update();
        return;
    }
    Vector3 diff = m_player->GetPlayerPos() - m_starPosition;
    // 接触判定
    if (diff.Length() <= 10.0f)
    {
        // Fadeを取得。
        Fade* fade = SceneManager::GetInstance()->GetFade();

        if (fade->GetFadeState() != FadeState::Fade_Out)
        {
            fade->StartFadeOut();
            m_isGoal = true;
        }

        return;
    }

    Rotation();
    m_starRender.SetScale(StarStatus::STAR_SCALE);
    m_starRender.SetPosition(m_starPosition);
    m_starRender.Update();
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
