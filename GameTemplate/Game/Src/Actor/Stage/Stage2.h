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
private:
    // ギミック。
    DimensionTrigger* pDimensionTrigger_ = nullptr;
    RotationFool* pRotationFool_ = nullptr;
    Box* pBox_ = nullptr;

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

class Stage2Slope : public IGameObject
{
public:
    Stage2Slope() {};
    virtual ~Stage2Slope() {};

    bool Start() override;
    void Update() override;
    void Render(RenderContext& rc) override;

public:
    inline void SetPos(const Vector3& pos)
    {
        basePos_ = pos;
        pos_ = pos;
    };

private:
    // 回転移動（円運動）用 ------------------
    void UpdateOrbit();

private:
    CollisionObject* pCollisionObject_ = nullptr;

private:
    ModelRender render_;
    // コリジョン Box のサイズ（斜面の長さに合わせて調整）
    Vector3 boxSize_ = Vector3(800.0f, 50.0f, 200.0f);// 現在角度
    Vector3 basePos_ = Vector3::Zero; // 円運動の中心
    Vector3 pos_ = Vector3::Zero; // 実際に使う位置
    Quaternion rot_ = Quaternion::Identity; // 斜面の傾き（モデル側で付けているなら Identity のままでOK）


    float orbitRadius_ = 400.0f;// 半径（後で調整）
    float orbitAngleDeg_ = 0.0f;// 現在角度
    float orbitSpeedDegPerSec_ = 30.0f;// 1秒あたりの角度

};

