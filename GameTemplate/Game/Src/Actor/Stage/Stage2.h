#pragma once
#include "Src/Actor/Stage/IStage.h"

namespace
{
    const Vector3 StartPos(98.077f, 140.0f,-60.0f);
}

class Box;
class DimensionTrigger;
class RotationFool;
class Player;
class CameraManager;
class Thwomp;
class TrackingEnemy;
class Star;
class Stage2 : public IStage
{
public:
	Stage2() {};
	virtual ~Stage2(){};

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;
    // モデルのファイルパスをセット。
    inline const std::string InitStage(const std::string& stagename)override
	{
		return IStage::InitStage(stagename);
	};
// ゲッター。
public:
    inline Vector3 GetStageStartPos() const override
    {
        return StartPos;
    }

    inline Vector3 SetPos(const Vector3&)
    {
        stagePos_ = stagePos_;
    };

private:
    // 敵。
    inline void TrackingInstance();         // 追尾型敵の生成。
    inline void ThwompInstance();           // トゥワンプ型敵の生成。
    // ギミック。
    inline void RotationFoolNewGO();        // 回転トリック。
    inline void BoxInstance();              // 箱モデルの生成。
    inline void DimensionTriggerInstance(); // トリガーモデルの生成。
    inline void StarInstance();
private:
    // ギミック。
    DimensionTrigger* pDimensionTrigger_ = nullptr;
    RotationFool* pRotationFool_ = nullptr;
    Box* pBox_ = nullptr;
    Star* pStar_ = nullptr;
    // 敵。
    TrackingEnemy* pTrackingEnemy_ = nullptr;
    Thwomp* pThwomp_ = nullptr;

    Player* pPlayer = nullptr;

private:
    ModelRender render_;
    Vector3 pos_ = Vector3::Zero;
    Vector3 scale_ = Vector3::One;
    Vector3 spawnPos_ = Vector3::Zero;

    float spawnTimer_ = 0.0f;
};


