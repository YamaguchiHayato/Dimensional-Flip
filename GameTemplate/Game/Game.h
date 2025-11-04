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
    void Render(RenderContext& rc);

private:
    // キャラクター。
    inline void EnemyNewGO_Tracking();

    // ギミック。
    inline void JumpPadNewGO();// ジャンプパッド。
    inline void StarNewGO();// スター。

    // UI。
    inline void UINewGO();
    // 各UIの生成。
    inline void TimerUINewGO();// タイマーUI。
    inline void NumberUINewGO();// ナンバーUI。
    inline void ScoreUINewGO();// スコアUI。
    inline void HPbarUINewGO();// HPbar。

    // その他。
    inline void FadeStart(); // フェード開始。
    inline void InitSkyCube(); // スカイキューブの初期化。
    inline void WallNewGO();   // 透明壁の生成。
    inline void DimensionTriggerNewGO(); // カメラの可動域制限。


private:
    // キャラクター。
    TrackingEnemy* pTrackingEnemy_ = nullptr;
    Player* pPlayer_ = nullptr;

    // カメラ。
    std::unique_ptr<CameraManager> pCameraManager_;
    

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
