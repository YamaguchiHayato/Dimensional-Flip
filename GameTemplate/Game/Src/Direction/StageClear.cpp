#include "stdafx.h"
#include "Fade.h"
#include "StageClear.h"
#include "Src/Actor/Stage/Gimmick/Star.h"
#include "Src/Core/StageManager.h"
#include "Src/Core/SceneManager.h"

bool StageClear::Start()
{
    StageID nextStageID = DecideStage();

    // Gameクラスを探索。
    app::core::Game* pGame = FindGO<app::core::Game>("game");
    if (pGame )
        pGame->RequestStageTransition(nextStageID);


    DeleteGO(this);
    return true;
}


StageID StageClear::DecideStage()
{
    // 1. StageManagerから「現在の」ステージIDを取得する。
    StageID currentStageID = StageManager::GetInstance()->GetCurrentStageID();

    // 2. 次のステージIDを決定するための変数。
    StageID nextStageID = currentStageID;

    // 3.現在のステージIDに戻づいて、次の遷移先を決定する。
    switch (currentStageID)
    {
    case StageID::sTutorialStage:
         nextStageID = StageID::sStage1;
         break;

    case StageID::sStage1:
         nextStageID = StageID::sInvalid;
         break;

    //case StageID::sStage2:
    //     nextStageID = StageID::sStage3;
    //     break;

    case StageID::sStageEX:
         nextStageID = StageID::sStageEX;
         break;

    default:
        break;
    }

    return nextStageID;
}

