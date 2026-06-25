#pragma once
#include "Src/Actor/Character/PlayerForward.h"


class Stage3;
class DimensionTrigger : public IGameObject
{
public:
	DimensionTrigger(){};
	virtual ~DimensionTrigger(){};

	bool Start();
	void Update();

	void Trigger();
    inline void SetStage(Stage3* pStage3)
    {
        pStage3_ = pStage3;
    }
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
    Stage3* pStage3_ = nullptr;

private:
	ModelRender triggerRender_;
	Vector3 triggerPos_;
	Quaternion trriggerRot_;

private:
	bool isPlayerInside_ = false;
};

