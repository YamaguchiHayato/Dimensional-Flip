#pragma once
#include "Src/Actor/Character/IState.h"

class PlayerFallState : public IState
{
private:
    Player* pPlayer_ = nullptr;

public:
    PlayerFallState(Player* player) : pPlayer_(player) {};
    virtual ~PlayerFallState() {};


public:
    void Enter()override;
    void Update()override;
    void Exit()override;
    bool RequestID(uint8_t& request)override;
};

