#pragma once
#include "Src/Actor/Stage/IStage.h"

// ギミック。
class JumpPad;
class Star;
class WallActor;
class DimensionTrigger;

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


private:
    // ギミック。
    JumpPad* pJumpPad_ = nullptr;
    Star* pStar_ = nullptr;
        
    Stage1* pStage1_ = nullptr;
    WallActor* pWall_ = nullptr;
    DimensionTrigger* pDimensionTrigger_ = nullptr;
};

