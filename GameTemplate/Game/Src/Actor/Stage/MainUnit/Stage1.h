#pragma once
#include "Src/Actor/Stage/IStage.h"

#include "Src/Actor/Character/Enemy/IEnemy.h"
#include "Src/Actor/Character/Enemy/EnemyFactory.h"

// ギミック。
class JumpPad;
class Star;
class Thwomp;
class WallActor;
class DimensionTrigger;
class RotationFool;
#include "Src/Actor/Character/PlayerForward.h"

namespace app{
    namespace stage{
        class Wall;
    }

    namespace enemy{
        class NormalEnemy;
    }

    namespace gimmick
    {
        class Switch;
        class TargetWall;
        class Block;
        class Coin;
    }
}

// メインクラス。
class Stage1 : public IStage
{
public:
	Stage1() = default;
	virtual ~Stage1();


public:
	bool Start()override;
	void Update()override;
	void Render(RenderContext & rc)override;


public:

    // モデルのファイルパスをセット。
	inline const std::string InitStage(const std::string& stagename)override
	{
		return IStage::InitStage(stagename);
	};

private:
    // ギミック生成。
    inline void JumpPadNewGO();          // ジャンプパッド。
    inline void StarNewGO();             // スター。
    inline void WallNewGO();             // 透明壁。
    inline void DimensionTriggerNewGO(); // カメラトリガー。
    inline void RotationFoolNewGO();     // 回転トリック。
    inline void WallCreateInstance();    // 壁インスタンス生成。
    void CreateSwitchGimmick();          // スイッチギミック生成。

    // 敵生成。
    void CreateNormalEnemy();            // 通常敵。
    void CreateTrackingEnemy();          // 追尾敵。
    void CreateThwompEnemy();            // 回転敵。
    void CreateFallEnenmy();             // 落下敵。

public:
    inline Vector3 GetStageStartPos() const override { return Vector3(0.0f, 20.0f, 0.0f); }


private:

    Player* pPlayer_ = nullptr;

    // ギミック。
    JumpPad* pJumpPad_ = nullptr;
    Star* pStar_ = nullptr;        
    WallActor* pWallActor_ = nullptr;
    app::stage::Wall* pWall_ = nullptr;
    DimensionTrigger* pDimensionTrigger_ = nullptr;


    app::gimmick::Switch* pSwitch_ = nullptr;             // スイッチ
    app::gimmick::TargetWall* pTargetWall_ = nullptr;     // ゴールの壁
    app::gimmick::Block* pBlock_ = nullptr;               // 足場ブロック
    std::vector<app::gimmick::Coin*> lAppearCoins_; // コインリスト
    int collectedCoinCount_ = 0;                          // 集めた枚数
    std::vector<app::gimmick::Block*> lBlocks_;

    std::vector<JumpPad*> lJumpPad_;
    std::vector<Star*> lStar_;
    std::vector<WallActor*> lWall_;
    std::vector<DimensionTrigger*> lDimensionTrigger_;
    std::vector<RotationFool*> lRotationFool_;
    std::vector<app::stage::Wall*> lWallInstance_;

    // 敵生成リスト。
    std::vector<app::enemy::IEnemy*> lEnemySpawnList_;

    Vector3 pos_ = Vector3::One;
    Vector3 scale_ = Vector3::One;
    Vector3 basePos_ = Vector3::Zero;
    Quaternion rot_ = Quaternion::Identity;
    Quaternion rot_90_Y = Quaternion::Identity;
};

