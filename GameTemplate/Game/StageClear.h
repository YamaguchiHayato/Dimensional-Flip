#pragma once
#include "stdint.h"
class Star;
class StageClear : public IGameObject {
public:
    StageClear() {}
    ~StageClear() {}

    bool Start() ;
    void Update();

public:
    // クリア状態を返す
    inline bool IsClear() const { return isClear_; }
    inline int GetStarCount() const
    {
        int count = 0;
        for (bool collected : starCollected_)
        {
            if (!collected) ++count;
        }
        return count;
    }
private:
    Star* pStar_ = nullptr;

private:
    bool isClear_ = false;
    std::vector<bool> starCollected_;
};
