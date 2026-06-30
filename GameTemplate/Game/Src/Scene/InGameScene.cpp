#include "stdafx.h"
#include "Src/Core/Game.h"
#include "Src/Scene/InGameScene.h"
#include "Src/Core/SceneManager.h"
#include "Src/Core/StageManager.h"
#include "Src/production/Fade.h"


InGameScene::~InGameScene()
{
    // Gameオブジェクトの削除。
    SceneManager::ResetRenderingStateForScene();
    DeleteGO(pGame_);
}

bool InGameScene::Start()
{
    pGame_ = NewGO<app::core::Game>(0, "game");

    SceneManager::GetInstance()->GetFade()->StartFadeIn();
    return true;
}


void InGameScene::Update()
{
    // フェードアウトが完了したらシーン遷移
    Fade* fade = SceneManager::GetInstance()->GetFade();
    if (fade && fade->IsFadeOutEnd())
    {
        // 現在のステージIDを確認
        StageID current = nsApp::nsStage::StageManager::GetInstance()->GetCurrentStageID();

        // 全ステージ（StageEX）をクリアした時だけリザルトへ
        if (current == StageID::sStageEX && pGame_->GetNextStageID() == StageID::sInvalid)
            SceneManager::GetInstance()->ChangeScene(SceneID::sEndRoll);
    }
}
