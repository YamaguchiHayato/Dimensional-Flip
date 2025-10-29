#include "stdafx.h"
#include "StageClear.h"
#include "Src/Actor/Stage/Gimmick/Star.h"

bool StageClear::Start() {
    return true;
}

void StageClear::Update()
{
    pStar_ = FindGO<Star>("star");

    // スターを全て取得したらクリア
    if (pStar_ && pStar_->GetStarCount() <= 0) {
        isClear_ = true;
    }
}
