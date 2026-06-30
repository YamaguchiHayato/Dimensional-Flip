#include "stdafx.h"
#include "Src/Scene/GameClearScene.h"
#include "Src/Core/SceneManager.h"
#include "Src/Production/Fade.h"

#include "Src/Core/StageManager.h"


GameClearScene::~GameClearScene()
{
    if (pGameClear_)
    {
        DeleteGO(pGameClear_);
        pGameClear_ = nullptr;  
    }

    // 背景クラスの削除。
    if (pSkyCube_)
    {
        DeleteGO(pSkyCube_);
        pSkyCube_ = nullptr;
    }
}

bool GameClearScene::Start()
{
    if (g_renderingEngine)
    {
        g_renderingEngine->EnableCompositeBackground(false);
        g_renderingEngine->SetStageBackGroundRenderer(nullptr);
    }

    // 背景クラスの生成。
    pSkyCube_ = NewGO<SkyCube>(0, "SkyCube");
    pSkyCube_->SetScale(Vector3::One * 1000.0f);
    pSkyCube_->SetType(EnSkyCubeType::enSkyCubeType_Wild);

    // カメラをセット。
    g_camera3D->SetPosition(Vector3::Zero);
    g_camera3D->SetTarget(Vector3::Zero);
    g_camera3D->Update();

    // ゲームクリア演出の生成。
    pGameClear_= NewGO<GameClear>(0, "gameClear");
    const auto& data = nsApp::nsStage::StageManager::GetStageResultData();
    pGameClear_->SetUpResultData(data);

    // フェードを取得。
    SceneManager::GetInstance()->GetFade()->StartFadeIn();
    return true;
}


void GameClearScene::Update()
{
    if (pGameClear_ && pGameClear_->IsFinished())
    {
        if (g_pad[0]->IsTrigger(enButtonA))
            SceneManager::GetInstance()->ChangeScene(SceneID::sTitle);
    }
}
