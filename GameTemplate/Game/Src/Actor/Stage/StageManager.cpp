#include "stdafx.h"
#include "Src/Actor/Character/Player.h"
#include "Src/Camera/CameraManager.h"

// ステージ。
#include "StageManager.h"
#include "Src/Actor/Stage/Stage1.h"
#include "Src/Actor/Stage/Stage2.h"

// ギミック。
#include "Src/Actor/Stage/Gimmick/IGimmic.h"
#include "Src/Actor/Stage/Gimmick/RotationFool.h"

#include "Src/Scene/SceneManager.h"
#include "Src/Scene/LoadingScene.h"
#include "Fade.h"

StageManager* StageManager::pStageManger_  =nullptr;

StageManager::~StageManager()
{
    // このポインタをシングルトンで削除。
    if (pStageManger_ == this)
    {
        pStageManger_ = nullptr;
    }

    DeleteGO(pCurrentStage_);
}

bool StageManager::Start()
{
    // 最初のシーンを生成する。
    pCurrentStage_ = CreateStage(StageID::sStage2);
    if (pCurrentStage_)
    {
        // ステージの初期値を設定する。
        currentID_s_ = StageID::sStage2;
        return true;
    }
    return false;
}

void StageManager::Update()
{
    // 常に現在のステージを更新する
    if (pCurrentStage_ != nullptr)
    {
        pCurrentStage_->Update();
    }
}

void StageManager::ChangeStageSync(StageID newStageID)
{
    // 1. 古いステージは削除する。
    if (pCurrentStage_ != nullptr)
    {
        DeleteGO(pCurrentStage_);
        pCurrentStage_ = nullptr;
    }

    // 2. 現在のステージとして設定する
    IStage* pNextStage = CreateStage(newStageID);
    if (pNextStage == nullptr)
    {
        return;
    }

    // 3. 現在のステージとして設定する。
    pCurrentStage_ = pNextStage;
    currentID_s_ = newStageID;
}

IStage* StageManager::CreateStage(StageID id)
{
    IStage* newStage = nullptr;
    switch (id)
    {
    case StageID::sStage1:
        newStage = NewGO<Stage1>(0, "stage1");
        break;

    case StageID::sStage2:
        newStage = NewGO<Stage2>(0, "stage2");
        break;
    default:
        break;
    }

    return newStage;
}

void StageManager::Render(RenderContext& rc)
{
    LoadingScene* pLoadingScene = FindGO<LoadingScene>("LoadingScene");

    if (pLoadingScene == nullptr && pCurrentStage_ != nullptr)
    {
//        pCurrentStage_->Render(rc);
    }
}

