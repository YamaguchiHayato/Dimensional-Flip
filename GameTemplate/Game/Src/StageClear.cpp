#include "stdafx.h"
#include "StageClear.h"
#include "Src/Actor/Stage/Gimmick/Star.h"
#include "Src/Actor/Stage/StageManager.h"

bool StageClear::Start() {
    StageManager::GetInstance()->ChangeStage(StageID::sStage2);
    return true;
}
