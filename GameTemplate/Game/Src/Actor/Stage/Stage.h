#pragma once
#include "Src//Actor/Actor.h"
/// <summary>
/// ステージをIDで管理するenum。
/// </summary>
enum class StageID
{
	ID_Stage1,
	ID_Stage2,
	ID_Stage3,
	ID_Stage4,
	Invalid
};

class Stage1; 
class Stage2;
class Stage3;
class Stage4;
/// <summary>
/// ステージを管理する親クラス。
/// </summary>
class Stage
{
public:
	Stage() {};
	~Stage() {};
	bool Start();
	void Update();
	void Render(RenderContext& rc);
};

/// <summary>
/// IGameObject継承用クラス。
/// </summary>
class IStage : public Actor
{
public:
	IStage() {};
	virtual ~IStage() {};
	virtual bool Start();
	virtual void Update();
	virtual void Render(RenderContext& rc);

	// ステージ用インスタンス。
	Stage1* m_stage1 = nullptr;
	Stage2* m_stage2 = nullptr;
	Stage3* m_stage3 = nullptr;
	Stage4* m_stage4 = nullptr;
};

