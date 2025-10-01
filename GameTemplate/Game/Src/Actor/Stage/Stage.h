#pragma once

/**
  * @brief ステージをIDで管理するenum。
  */
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

/**
  * @brief ステージを管理する親クラス。
  */
class Stage : public IGameObject
{
public:
	Stage() {};
	~Stage() {};
	bool Start() override {}
	void Update() override {}
	void Render(RenderContext& rc) override {}
};

/**
  * @brief IGameObject継承用クラス。
  */
class IStage : public IGameObject
{
public:
	IStage() {};
	virtual ~IStage() {};
	virtual bool Start() override { return true; }
	virtual void Update() override {}
	virtual void Render(RenderContext& rc) override {}

	// ステージ用インスタンス。
	Stage1* m_stage1 = nullptr;
	Stage2* m_stage2 = nullptr;
	Stage3* m_stage3 = nullptr;
	Stage4* m_stage4 = nullptr;
};

