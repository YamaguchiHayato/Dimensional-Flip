#pragma once
#include "Src/Actor/Stage/IStage.h"

// ギミック。
class JumpPad;
class Star;
class WallActor;
class DimensionTrigger;
class RotationFool;

class Player;

namespace app{
    namespace stage{
        class Wall;
    }
}

// メインクラス。
class Stage1 : public IStage
{
public:
	Stage1() = default;
	virtual ~Stage1();
	bool Start()override;
	void Update()override;
	void Render(RenderContext & rc)override;

    // Playerがカメラアクションをしたかチェックする関数。
    void CheckCameraAction();

    // モデルのファイルパスをセット。
	inline const std::string InitStage(const std::string& stagename)override
	{
		return IStage::InitStage(stagename);
	};

private:
    // ギミック生成。
    inline void JumpPadNewGO();// ジャンプパッド。
    inline void StarNewGO();// スター。
    inline void WallNewGO();// 透明壁。
    inline void DimensionTriggerNewGO(); // カメラトリガー。
    inline void RotationFoolNewGO();     // 回転トリック。
    inline void WallCreateInstance();    // 壁インスタンス生成。

public:
    inline Vector3 GetStageStartPos() const override { return Vector3(0.0f, 100.0f, 0.0f); }


private:

    Player* pPlayer_ = nullptr;

    // ギミック。
    JumpPad* pJumpPad_ = nullptr;
    Star* pStar_ = nullptr;        
    WallActor* pWallActor_ = nullptr;
    app::stage::Wall* pWall_ = nullptr;
    DimensionTrigger* pDimensionTrigger_ = nullptr;


    std::vector<JumpPad*> lJumpPad_;
    std::vector<Star*> lStar_;
    std::vector<WallActor*> lWall_;
    std::vector<DimensionTrigger*> lDimensionTrigger_;
    std::vector<RotationFool*> lRotationFool_;
    std::vector<app::stage::Wall*> lWallInstance_;

    Vector3 scale_ = Vector3::One;
    Vector3 basePos_ = Vector3::Zero;
    Quaternion rot_ = Quaternion::Identity;
    Quaternion rot_90_Y = Quaternion::Identity;


};

