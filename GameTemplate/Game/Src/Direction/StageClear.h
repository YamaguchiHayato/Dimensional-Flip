#pragma once
#include "stdint.h"
#include "Game.h"
#include "Src/Actor/Stage/IStage.h"


class StageClear : public IGameObject {
public:
    StageClear() {}
    ~StageClear() {}

    bool Start() ;
private:
    StageID DecideStage();


};
