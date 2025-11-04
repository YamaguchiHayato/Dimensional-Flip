#pragma once
#include "stdint.h"
class Star;
class StageClear : public IGameObject {
public:
    StageClear() {}
    ~StageClear() {}

    bool Start() ;

public:
    // クリア状態を返す
    inline bool IsClear() const { return isClear_; }
private:
    Star* pStar_ = nullptr;

private:
    bool isClear_ = false;
    std::vector<bool> starCollected_;
};
