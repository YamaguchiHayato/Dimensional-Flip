#pragma once
#include "Level3DRender/LevelRender.h"
#include "Src/Camera/CameraManager.h"

#include <DirectXTK/Inc/Audio.h>
#include <memory>    
class CameraManager;
class Dimensiontrigger;
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
    // キャラクター。
    inline void EnemyNewGO_Tracking();

    // ギミック。
    inline void JumpPadNewGO();
    void StarNewGO();

    // 各UIの生成。
    inline void UINewGO();
    inline void TimerUINewGO();
    inline void NumberUINewGO();
    inline void ScoreUINewGO();
    inline void HPbarUINewGO();

    // その他。
    inline void FadeStart();
    inline void InitSkyCube();
    inline void WallNewGO();
    inline void DimensionTriggerNewGO();


private:
    // キャラクター。
    TrackingEnemy* pTrackingEnemy_ = nullptr;
    Player* pPlayer_ = nullptr;

    // カメラ。
    std::unique_ptr<CameraManager> pCameraManager_;

    std::unique_ptr<DirectX::AudioEngine> audioEngine_;

    // ステージ。
    Stage1* pStage1_ = nullptr;
    Star* pStar_ = nullptr;

    // ギミック。
    WallActor* pWallActor_ = nullptr;
    JumpPad* pJumpPad_ = nullptr;

    // UI。
    TimerUI* pTimerUI_ = nullptr;
    NumberUI* pNumberUI_ = nullptr;
    ScoreUI* pScoreUI_ = nullptr;
    HPbarUI* pHpbarUI_ = nullptr;

    // その他。
    Fade* pFade_ = nullptr;
    SkyCube* pSkyCube_ = nullptr;
    Dimensiontrigger* pDimensionTrigger_ = nullptr;
};
