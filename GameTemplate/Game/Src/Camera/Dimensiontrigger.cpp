#include "stdafx.h"
#include "Src/Actor/Character/Player.h"
#include "Src/Camera/CameraManager.h"
#include "DimensionTrigger.h"

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
	if(!pPlayer_ || !pTriggerObject_) return; // 両方のポインタをチェック

	// コリジョン。
	pTriggerObject_->SetPosition(triggerPos_ + COLLISION_HEIGHT);

	// 現在ヒットしているかどうか
	bool isCurrentlyHit = pTriggerObject_->IsHit(pPlayer_->GetPlayerCC());

	// 状態の変化をチェック
	if (isCurrentlyHit && ! isPlayerInside_) 
		// 【入った瞬間】カウンターを増やす
		pPlayer_->EnterTriggerArea();
	else if (!isCurrentlyHit && isPlayerInside_)
		// 【出た瞬間】カウンターを減らす
		pPlayer_->ExitTriggerArea();

    // ボス部屋前のトリガー処理 (Pos4: 3600.0, 39.2f, -3927.0f)
    if (std::abs(triggerPos_.x - 3600.0f) < 10.0f && std::abs(triggerPos_.z - (-3927.0f)) < 10.0f)
    {
        CameraManager* pCameraManager = FindGO<CameraManager>("CameraManager");
        if (pCameraManager)
        {
            // プレイヤーがこのトリガーに「入った瞬間」だけ反応
            if (isCurrentlyHit && !isPlayerInside_)
            {
                // RequestBossModeを呼び出す
                // これにより、強制回転(90度)とBossCameraStrategyへの切り替えが実行される
                pCameraManager->RequestBossMode();
            }
        }
    }
    isPlayerInside_ = isCurrentlyHit;
}
