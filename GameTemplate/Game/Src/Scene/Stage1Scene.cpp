#include "stdafx.h"
#include "Src/Actor/Character/Player.h"
#include "Src/Actor/Stage/Gimmick/Star.h"
#include "Src/Actor/Stage/Stage1.h"
#include "Src/Scene/Stage1Scene.h"
#include "Src/Scene/SceneManager.h"
#include "Src/Title.h"
#include "StageClear.h"
#define SMGetIns SceneManager::GetInstance // シングルトンインスタンスを取得するマクロ定義

Stage1Scene::~Stage1Scene()
{
    // 管理しているオブジェクトの削除。
    if (pStage1_)DeleteGO(pStage1_);
    if (pPlayer_)DeleteGO(pPlayer_);
}

// ステージ1シーン::初期化処理。
bool Stage1Scene::Start()
{
	// ステージ1の初期化処理を行う。
	pStage1_ = NewGO<Stage1>(0, "Stage1");
	pPlayer_ = NewGO<Player>(0, "player");

    pStar_ = FindGO<Star>("star");
	pStageClear_ = FindGO<StageClear>("stageclear");

	return (pStage1_ != nullptr && pPlayer_ != nullptr);
}

void Stage1Scene::Update()
{
    // ステージクリアの判定。
   if (pStageClear_ && pStageClear_->IsClear())
       // InGameシーンへ遷移する。
		SceneManager::GetInstance()->ChangeScene(SceneID::sInGame);
}

