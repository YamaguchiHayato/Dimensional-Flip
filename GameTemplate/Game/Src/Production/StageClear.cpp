#include "stdafx.h"
#include "Fade.h"
#include "StageClear.h"
#include "Src/Actor/Stage/Gimmick/StageGimmick/Star.h"
#include "Src/Core/StageManager.h"
#include "Src/Core/SceneManager.h"

bool StageClear::Start()
{
    stageClearSprite_.Init("Assets/sprite/stageClear.DDS", 324, 214);
    stageClearSprite_.SetScale(Vector3::One);


    return true;
}


void StageClear::Update()
{
    timer_ += g_gameTime->GetFrameDeltaTime();

    stageClearSprite_.SetScale(Vector3::One);
    stageClearSprite_.Update();
}


void StageClear::Render(RenderContext& rc)
{
    stageClearSprite_.Draw(rc);
}


StageID StageClear::DecideStage()
{
    // 1. StageManagerから「現在の」ステージIDを取得する。
    StageID currentStageID = nsApp::nsStage::StageManager::GetInstance()->GetCurrentStageID();

    // 2. 次のステージIDを決定するための変数。
    StageID nextStageID = currentStageID;

    // 3.現在のステージIDに戻づいて、次の遷移先を決定する。
    switch (currentStageID)
    {
    case StageID::sTutorialStage:
         nextStageID = StageID::sStage1;
         break;

    case StageID::sStage1:
         nextStageID = StageID::sStageEX;
         break;

    case StageID::sStageEX:
         nextStageID = StageID::sInvalid;
         break;

    default:
        break;
    }

    return nextStageID;
}

