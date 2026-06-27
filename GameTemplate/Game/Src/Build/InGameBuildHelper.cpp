#include "stdafx.h"

#include "InGameBuildHelper.h"
#include "Src/Actor/Stage/BackGround/BossBackGround.h"
#include "Src/Actor/Stage/BackGround/ScrollStageBackGround.h"

namespace nsApp
{
    void InGameBuildHelper::Initialize(StageID stageID)
    {
        stageID_ = stageID;
        pBackGround_ = nullptr;

        buildFunctions_.clear();
        currentBuildIndex_ = 0;
        isFinished_ = false;
        isLoadSuccess_ = true;

        InitializeBuildFunctions();
    }

    void InGameBuildHelper::InitializeBuildFunctions()
    {
        buildFunctions_.clear();

        buildFunctions_.push_back([this]() { BuildParameters(); });
        buildFunctions_.push_back([this]() { BuildBackGroundStep(); });
        buildFunctions_.push_back([this]() { FinishBuild(); });
    }

    void InGameBuildHelper::ExecuteNextBuildFunction()
    {
        if (isFinished_)
            return;

        if (currentBuildIndex_ >= static_cast<int>(buildFunctions_.size()))
        {
            isFinished_ = true;
            return;
        }

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
        pBackGround_ = CreateBackGround(stageID_);

        if (pBackGround_ == nullptr)
            OutputDebugStringA("InGameBuildHelper::BuildBackGroundStep - CreateBackGround returned nullptr.\n");
    }

    nsStage::nsBackGround::IBackGround* InGameBuildHelper::CreateBackGround(StageID stageID)
    {
        switch (stageID)
        {
        case StageID::sTutorialStage:
        case StageID::sStage1:
            return NewGO<nsApp::nsStage::nsScrollBackGround::ScrollStageBackGround>(0, "Normal");

        case StageID::sStageEX:
            return NewGO<nsApp::nsStage::nsBackGround::BossBackGround>(0, "Boss");

        default:
            return nullptr;
        }
    }

    void InGameBuildHelper::FinishBuild()
    {
        isFinished_ = true;
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
