#include "stdafx.h"
#include "Src/Actor/Character/Player.h"
#include "Src/Core/CameraManager.h"
#include "DimensionTrigger.h"
#include "Src/Actor/Stage/Stage3.h"
namespace
{
	const Vector3 COLLISION_HEIGHT(0.0f, 50.0f, 0.0f);//コリジョンの高さ
	const Vector3 COLLISION_SIZE(100.0f, 100.0f, 100.0f);//コリジョンの大きさ
}

bool DimensionTrigger::Start()
{
	pPlayer_ = FindGO<Player>("player");
	if(!pPlayer_) {
		K2_LOG("DimensionTrigger::Start() プレイヤーが見つかりません。\n");
		return false;
	}

	pTriggerObject_ = NewGO<CollisionObject>(0, "collisionobject");

	//コリジョンを動く床に設置
	pTriggerObject_->CreateBox
	(   triggerPos_ + COLLISION_HEIGHT,
		Quaternion::Identity,
		COLLISION_SIZE );

	pTriggerObject_->SetIsEnableAutoDelete(false);

	return true;
}

void DimensionTrigger::Update()
{
    pPlayer_ = FindGO<Player>("player");
    if (pPlayer_==nullptr) return;

    Trigger();

	//レンダラーの更新
	triggerRender_.SetPosition(triggerPos_);
	triggerRender_.SetAlpha(0.0f);
	triggerRender_.SetRotation(trriggerRot_);
	triggerRender_.Update();

}

void DimensionTrigger::Trigger()
{
    if (!pPlayer_ || !pTriggerObject_)
        return;

    pTriggerObject_->SetPosition(triggerPos_ + COLLISION_HEIGHT);
    bool isCurrentlyHit = pTriggerObject_->IsHit(pPlayer_->GetPlayerCC());

    // 状態の変化をチェック
    if (isCurrentlyHit && !isPlayerInside_)
        // 【入った瞬間】
        pPlayer_->EnterTriggerArea();

    else
        // 【出た瞬間】
        pPlayer_->ExitTriggerArea();

    isPlayerInside_ = isCurrentlyHit;
}
