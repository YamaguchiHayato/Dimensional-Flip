#pragma once

class IState
{
public:
    IState() {};
    virtual ~IState() {};


public:
    // 1回処理される処理。
    virtual void Enter() = 0;
    // 更新処理。
    virtual void Update() = 0;
    // 
    virtual void Exit() = 0;
    // ステート遷移要求処理。
    virtual bool RequestID(uint8_t& request) = 0;
};

