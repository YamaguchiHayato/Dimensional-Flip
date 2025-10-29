#pragma once
class Player;
class DimensionTrigger : public IGameObject
{
public:
	DimensionTrigger(){};
	virtual ~DimensionTrigger(){};

	bool Start();
	void Update();

	void Trigger();

// セッター。
public:
	inline void SetTriggerPos(const Vector3& triggerPos) 
	{
		triggerPos_ = triggerPos; 
	}
// セッターここまで。

private:
	CollisionObject* pTriggerObject_ = nullptr;
	Player* pPlayer_ = nullptr;

private:
	ModelRender triggerRender_;
	Vector3 triggerPos_;
	Quaternion trriggerRot_;

private:
	bool isPlayerInside_ = false;
};

