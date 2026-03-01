#include "stdafx.h"
#include "Src/Scene/GameClearScene.h"
#include "Src/Core/SceneManager.h"
#include "Src/Production/Fade.h"

#include "Src/Core/StageManager.h"
#include "Src/Actor/Stage/BackGround.h"


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
    // 背景クラスの生成。
    pSkyCube_ = NewGO<SkyCube>(0, "SkyCube");
    pSkyCube_->SetScale(Vector3::One * 1000.0f);
    pSkyCube_->SetType(EnSkyCubeType::enSkyCubeType_Wild);
    g_renderingEngine->SetAmbientByIBLTexture(pSkyCube_->GetTextureFilePath(),1.0f);

    // カメラをセット。
    g_camera3D->SetPosition(Vector3::Zero);
    g_camera3D->SetTarget(Vector3::Zero);
    g_camera3D->Update();

    // ゲームクリア演出の生成。
    pGameClear_= NewGO<GameClear>(0, "gameClear");
    const auto& data = app::core::StageManager::GetStageResultData();
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
        {
            // 演出が終わっていて、かつボタンが押されていたら。
            SceneManager::GetInstance()->ChangeScene(SceneID::sTitle);
        }
    }
}
