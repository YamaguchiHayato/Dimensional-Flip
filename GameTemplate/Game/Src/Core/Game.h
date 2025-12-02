#pragma once
#include "stdint.h"
#include "Level3DRender/LevelRender.h"
#include "Src/Core/CameraManager.h"
#include "Src/Core/SceneManager.h"
#include "Src/Actor/Stage/IStage.h"
#include "Src/UI/HPbarUI.h"
#include "Src/UI/NumberUI.h"
#include "Src/UI/ScoreUI.h"
#include "Src/UI/TimerUI.h"


#include <DirectXTK/Inc/Audio.h>
#include <memory>

class CameraManager;
class Player;
class TrackingEnemy;
class Fade;
class LoadingScene;

// UI。
class NumberUI;
class ScoreUI;
class TimerUI;
class HPbarUI;

class Player;

// データ統合クラス。
class Game : public IGameObject
{
public:
    Game() = default;
    virtual ~Game();

    bool Start();
    void Update();

    // ステージ遷移要求。
    void RequestStageTransition(StageID nextStageID);
private:
    // Player。
    void PlayerInstance();
    // UI。
    inline void UIInstance();
    // 各UIの生成。
    // タイマーUI。
    inline void TimerInstance();
    // ナンバーUI。
    inline void NumberInstance();
    // スコアUI。
    inline void ScoreInstance();
    // HPbar。
    inline void HPbarInstance();
    // その他。
    void InitSkyCube(); // スカイキューブの初期化。
    // 遷移時の更新処理。
    void UpdateTransition();

private:
    std::unique_ptr<CameraManager> pCameraManager_;
    TimerUI* pTimerUI_ = nullptr;
    NumberUI* pNumberUI_ = nullptr;
    ScoreUI* pScoreUI_ = nullptr;
    HPbarUI* pHpbarUI_ = nullptr;
    Fade* pFade_ = nullptr;
    LoadingScene* pLoadingScene_ = nullptr;
    SkyCube* pSkyCube_ = nullptr;
    Player* pPlayer_ = nullptr;
  

    // 遷移管理用メンバ。
    SceneTransitionState state_ = SceneTransitionState::None;
    StageID nextStageID_ = StageID::sInvalid;
    Stopwatch stageClearTimer_;
private:
    // 現在のステージ番号を追跡するための変数。
    uint8_t currentStageNum_ = -1;
};
