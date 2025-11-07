#pragma once
#include "Level3DRender/LevelRender.h"
#include "Src/Camera/CameraManager.h"
#include "stdint.h"

#include <DirectXTK/Inc/Audio.h>
#include <memory>

class CameraManager;
class Dimensiontrigger;
class TrackingEnemy;
class Fade;
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
    Game() {};
    virtual ~Game(){};
    bool Start();
    void Update();
    void Render(RenderContext& rc);

private:

    // UI。
    inline void UIInstance();
    // 各UIの生成。
    inline void TimerInstance();// タイマーUI。
    inline void NumberInstance();// ナンバーUI。
    inline void ScoreInstance();// スコアUI。
    inline void HPbarInstance();// HPbar。

    // その他。
    inline void FadeStart(); // フェード開始。
    inline void InitSkyCube(); // スカイキューブの初期化。
    inline void WallNewGO();   // 透明壁の生成。
    inline void DimensionTriggerNewGO(); // カメラの可動域制限。

    inline void EnemyNewGO_Tracking(); // 追尾型敵の生成。

    // ステージが変更された時に呼ばれる処理。
    void OnStageChange(uint8_t newStageNum);

    inline void ApplyStageSpecifics(uint8_t newStageNum);
private:

    // カメラ。
    std::unique_ptr<CameraManager> pCameraManager_;
    
    // UI。
    TimerUI* pTimerUI_ = nullptr;
    NumberUI* pNumberUI_ = nullptr;
    ScoreUI* pScoreUI_ = nullptr;
    HPbarUI* pHpbarUI_ = nullptr;

    // その他。
    Fade* pFade_ = nullptr;
    SkyCube* pSkyCube_ = nullptr;

    TrackingEnemy* pTrackingEnemy_ = nullptr;
    Player* pPlayer_ = nullptr;

private:
    // 現在のステージ番号を追跡するための変数。
    uint8_t currentStageNum_ = -1;
};
