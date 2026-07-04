#include "stdafx.h"

#include "InGameBuildHelper.h"
#include "Src/Actor/Stage/BackGround/ScrollStageBackGround.h"
#include "Src/Parameter/Stage/StageMasterTable.h"
#include "Src/Presentation/UI/Screens/BossHubScreenHost.h"
#include "Src/Presentation/UI/Screens/BossHubScreen.h"

namespace nsApp
{
    void InGameBuildHelper::Initialize(nsStage::StageID stageID)
    {
        /* 初期化処理。*/
        stageID_ = stageID;
        pBackGround_ = nullptr;
        buildFunctions_.clear();
        currentBuildIndex_ = 0;
        isFinished_ = false;
        isLoadSuccess_ = true;

        /* ビルド関数の初期化。*/
        InitializeBuildFunctions();
    }


    void InGameBuildHelper::InitializeBuildFunctions()
    {
        /* ビルド関数のリストを初期化。*/
        buildFunctions_.clear();
        buildFunctions_.push_back([this]() { BuildParameters(); });
        buildFunctions_.push_back([this]() { FinishBuild(); });
        buildFunctions_.push_back([this]() { BuildBossHudUi(); });
    }


    void InGameBuildHelper::ExecuteNextBuildFunction()
    {
        /* ビルド関数を順番に実行。*/
        if (isFinished_)
            return;

        /* 現在のビルド関数を実行。*/
        if (currentBuildIndex_ >= static_cast<int>(buildFunctions_.size()))
        {
            isFinished_ = true;
            return;
        }

        /* 現在のビルド関数を実行。*/
        buildFunctions_[currentBuildIndex_]();
        ++currentBuildIndex_;
    }


    void InGameBuildHelper::BuildParameters()
    {
        isLoadSuccess_ = parameterSystem_.LoadAll();
        if (!isLoadSuccess_)
            OutputDebugStringA("InGameBuildHelper::BuildParameters - LoadAll failed.\n");
    }

    void InGameBuildHelper::BuildBackGroundStep()
    {
        /* ステージ背景を生成。*/
        pBackGround_ = CreateBackGround(stageID_);
        if (pBackGround_ == nullptr)
            OutputDebugStringA("InGameBuildHelper::BuildBackGroundStep - CreateBackGround returned nullptr.\n");
    }



    nsStage::nsBackGround::IBackGround* InGameBuildHelper::CreateBackGround(nsStage::StageID stageID)
    {
        const auto& master = nsSystem::StageMasterTable::Get(stageID);
        if (master.stageID == nsStage::StageID::sInvalid)
            return nullptr;

        if (master.backgroundType != "Scroll" && master.backgroundType != "Boss")
            return nullptr;

        nsStage::nsScrollBackGround::ScrollStageBackGround::SetPendingStageID(stageID);

        auto* bg = NewGO<nsStage::nsScrollBackGround::ScrollStageBackGround>(0, "BackGround");
        if (bg != nullptr)
            bg->SetStageID(stageID);

        return bg;
    }


    void InGameBuildHelper::FinishBuild()
    {
        isFinished_ = true;
    }


    void InGameBuildHelper::BuildBossHudUi()
    {
        /* 二重生成防止。*/
        if (pBossHudHost_ != nullptr)
            return;

        /* ボス HUD スクリーンホストを生成。*/
        pBossHudHost_ = NewGO<nsUI::BossHudScreenHost>(1, "BossHudScreenHost");

        if (pBossHudHost_ != nullptr)
            return;

        /* ボス HUD スクリーンを取得して、非表示にする。*/
        if (auto* pScreen = pBossHudHost_->GetBossHudScreen())
        {
            pScreen->SetVisible(false);
            bossHudData_.SetScreen(pScreen);
            pScreen->Bind(&bossHudData_);
        }
    }


    float InGameBuildHelper::GetProgress() const
    {
        if (buildFunctions_.empty())
            return isFinished_ ? 1.0f : 0.0f;

        float progress = static_cast<float>(currentBuildIndex_) / static_cast<float>(buildFunctions_.size());
        if (progress < 0.0f)
            return 0.0f;
        if (progress > 1.0f)
            return 1.0f;
        return progress;
    }
} // namespace nsApp
