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

StageManager* StageManager::pStageManger_  =nullptr;

bool StageManager::Start()
{
    // 最初のシーンを生成する。
    pCurrentStage_ = CreateStage(StageID::sStage1);
    if (pCurrentStage_)
    {
        pCurrentStage_->Start();
        // ステージの初期値を設定する。
        currentID_s_ = StageID::sStage1;
        return true;
    }
    return false;
}

void StageManager::Update()
{
    if (requestID_s_ != StageID::sInvalid)
    {
        IStage* nextStage = CreateStage(requestID_s_);

        if (nextStage != nullptr)
        {
            nextStage->Start();
            // 前のステージを消す。
            delete pCurrentStage_;

            // 次のステージに切り替える。
            pCurrentStage_ = nextStage;

            // ステージIDを更新する。
            currentID_s_ = requestID_s_;
        }
        // ステージリクエストをクリア
        requestID_s_ = StageID::sInvalid;
    }


    // ステージの更新処理。
    if (pCurrentStage_ != nullptr)
    {
        pCurrentStage_->Update();
    }
}

void StageManager::Render(RenderContext& rc)
{
    if (pCurrentStage_ != nullptr)
        pCurrentStage_->Render(rc);
}

IStage* StageManager::CreateStage(StageID id)
{
    IStage* newStage = nullptr;
    switch (id)
    {
    case StageID::sStage1:
        newStage = new Stage1();
        break;

    case StageID::sStage2:
        newStage = new Stage2();
        break;
    default:
        break;
    }

    return newStage;
}
