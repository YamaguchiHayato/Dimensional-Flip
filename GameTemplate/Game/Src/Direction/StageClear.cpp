#include "stdafx.h"
#include "Fade.h"
#include "StageClear.h"
#include "Src/Actor/Stage/Gimmick/Star.h"
#include "Src/Actor/Stage/StageManager.h"
#include "Src/Scene/SceneManager.h"

bool StageClear::Start()
{
//   StageManager::GetInstance()->ChangeStage(StageID::sStage2);
    StageID nextStageID = DecideStage();

    // Gameクラスを探索。
    Game* pGame = FindGO<Game>("game");
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
    case StageID::sStage1:
         nextStageID = StageID::sStage2;
         break;

    case StageID::sStage2:
         nextStageID = StageID::sStage3;
         break;

    case StageID::sStage3:
         nextStageID = StageID::sStageEX;
         break;

    default:
        break;
    }

    return nextStageID;
}

