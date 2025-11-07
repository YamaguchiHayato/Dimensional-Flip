#pragma once
#include "Src/Actor/Stage/IStage.h"

class Player;
// ギミック。
class JumpPad;
class Star;
class RotationFool;
class CameraManager;
// UI。
class NumberUI;
class ScoreUI;
class TimerUI;
class HPbarUI;

// メインクラス。
class Stage1 : public IStage
{
public:
	Stage1() {};
	~Stage1();
	bool Start()override;
	void Update()override;
	void Render(RenderContext & rc)override;
    // モデルのファイルパスをセット。
	inline const std::string InitStage(const std::string& stagename)override
	{
		return IStage::InitStage(stagename);
	};
    inline Vector3 GetStageStartPos() const override
    {
        return Vector3(0.0f, 100.0f, 0.0f);
    }

private:
    // ギミック生成。
    inline void JumpPadNewGO();// ジャンプパッド。
    inline void StarNewGO();// スター。
    inline void WallNewGO();// 透明壁。
    inline void DimensionTriggerNewGO(); // カメラトリガー。
    // プレイヤー生成。
    inline void PlayerNewGO(); // プレイヤー。
    // UI。
    inline void UIInstance();
    // 各UIの生成。
    inline void TimerInstance();// タイマーUI。
    inline void NumberInstance();// ナンバーUI。
    inline void ScoreInstance();// スコアUI。
    inline void HPbarInstance();// HPbar。

private:
    Player* pPlayer_ = nullptr;

    std::unique_ptr<CameraManager> pCameraManager_;


    // ギミック。
    JumpPad* pJumpPad_ = nullptr;
    Star* pStar_ = nullptr;
    RotationFool* pRotationFool_ = nullptr;

};

