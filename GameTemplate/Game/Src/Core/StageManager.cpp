#include "stdafx.h"

// キャラクタークラス。
#include "Src/Actor/Character/Player/Player.h"


// カメラクラス。
#include "Src/Core/CameraManager.h"


// ステージ。
#include "Src/Actor/Stage/MainUnit/Stage1.h"
#include "Src/Actor/Stage/MainUnit/Stage2.h"
#include "Src/Actor/Stage/MainUnit/Stage3.h"
#include "Src/Actor/Stage/MainUnit/StageEX.h"
#include "Src/Actor/Stage/MainUnit/TutorialStage.h"
#include "Src/Core/StageManager.h"

// 演出クラス。
#include "Src/Direction/Fade.h"

// ギミック。
#include "Src/Actor/Stage/Gimmick/IGimmic.h"
#include "Src/Actor/Stage/Gimmick/RotationFool.h"

// ゲームシーン。  
#include "Src/Core/SceneManager.h"
#include "Src/Scene/LoadingScene.h"



StageManager* StageManager::pStageManger_  =nullptr;


StageManager::~StageManager()
{
    // このポインタをシングルトンで削除。
    if (pStageManger_ == this)
        pStageManger_ = nullptr;

    DeleteGO(pCurrentStage_);
}


bool StageManager::Start()
{
    // 最初のシーンを生成する。
    pCurrentStage_ = CreateStage(StageID::sStage1);
    if (pCurrentStage_)
    {
        // ステージの初期値を設定する。
        stageCurrentID_ = StageID::sStage1;
        return true;
    }
    return false;
}


void StageManager::Update()
{
    if (stageRequestID_ != StageID::sInvalid)
    {
        ChangeStageSync(stageRequestID_);
        stageRequestID_ = StageID::sInvalid;
    }

    if (pCurrentStage_ != nullptr)
        pCurrentStage_->Update();
}


void StageManager::ChangeStageSync(StageID newStageID)
{
    // 0. すでに同じステージなら何もしない
    if (stageCurrentID_ == newStageID && pCurrentStage_ != nullptr)
    {
        OutputDebugStringA("ChangeStageSync: same stage, skip.\n");
        return;
    }

    OutputDebugStringA("ChangeStageSync: start.\n");

    // 1. 今のステージを確実に消す
    if (pCurrentStage_ != nullptr)
    {
        OutputDebugStringA("  DeleteGO(pCurrentStage_)\n");
        DeleteGO(pCurrentStage_);
        pCurrentStage_ = nullptr;
    }

    if (auto s1 = FindGO<Stage1>("stage1"))
    {
        OutputDebugStringA("  DeleteGO(stage1)\n");
        DeleteGO(s1);
    }
    if (auto s2 = FindGO<Stage2>("stage2"))
    {
        OutputDebugStringA("  DeleteGO(stage2)\n");
        DeleteGO(s2);
    }
    if (auto s3 = FindGO<Stage3>("stage3"))
    {
        OutputDebugStringA("  DeleteGO(stage3)\n");
        DeleteGO(s3);
    }
    if (auto ex = FindGO<app::stage::StageEX>("stageEX"))
    {
        OutputDebugStringA("  DeleteGO(stageEX)\n");
        DeleteGO(ex);
    }

    if (auto tutorial = FindGO<app::stage::TutorialStage>("tutorialStage"))
    {
        OutputDebugStringA("  DeleteGO(tutorialStage)\n");
        DeleteGO(tutorial);
    }

    // 2. 新しいステージを生成
    IStage* pNextStage = CreateStage(newStageID);
    if (pNextStage == nullptr)
    {
        OutputDebugStringA("  CreateStage FAILED\n");
        return; 
    }

    // 3. 現在のステージとして登録
    pCurrentStage_ = pNextStage;
    stageCurrentID_ = newStageID;

    OutputDebugStringA("ChangeStageSync: end.\n");
}


IStage* StageManager::CreateStage(StageID id)
{
    switch (id)
    {
    case StageID::sTutorialStage:
         pCurrentStage_ = NewGO<app::stage::TutorialStage>(0, "tutorialStage");
         break;

    case StageID::sStage1:
         pCurrentStage_ = NewGO<Stage1>(0, "stage1");
         break;

    case StageID::sStage2:
         pCurrentStage_ = NewGO<Stage2>(0, "stage2");
         break;

    case StageID::sStage3:
         pCurrentStage_ = NewGO<Stage3>(0, "stage3");
         break;

    case StageID::sStageEX:
         pCurrentStage_ = NewGO<app::stage::StageEX>(0, "stageEX");
         break;

    default:
        break;
    }

    return pCurrentStage_;
}


void StageManager::Render(RenderContext& rc)
{
    LoadingScene* pLoadingScene = FindGO<LoadingScene>("LoadingScene");

    if (pLoadingScene == nullptr && pCurrentStage_ != nullptr)
    {}
}

