#pragma once
#include "Level3DRender/LevelRender.h"
class Fade;
class TrackingEnemy;
class Player;
class NumberUI;
class ScoreUI;
class TimerUI;
class HPbarUI;
class WallActor;
class Stage1;
class Star;
class JumpPad;
class GameCamera;
class Game : public IGameObject
{
public:
    Game() {};
    virtual ~Game();
    bool Start();
    void Update();

private:
    void EnemyNewGO_Tracking();

    void WallNewGO();

    void JumpPadNewGO();

    void StarNewGO();

    void UINewGO();

    void TimerUINewGO();

    void NumberUINewGO();

    void ScoreUINewGO();

    void HPbarUINewGO();
    void FadeStart();
    void SetupViewRotationAreas();
    void InitSkyCube();

private:
    TrackingEnemy* trackingEnemy_ = nullptr;
    Player* player_ = nullptr;

    Stage1* stage1_ = nullptr;
    Star* star_ = nullptr;
    WallActor* wallActor_ = nullptr;
    JumpPad* jumpPad_ = nullptr;

    TimerUI* timerUI_ = nullptr;
    NumberUI* numberUI_ = nullptr;
    ScoreUI* scoreUI_ = nullptr;
    HPbarUI* hpbarUI_ = nullptr;

    Fade* fade_ = nullptr;
    GameCamera* gameCamera_ = nullptr;
    SkyCube* skyCube_ = nullptr;
};
