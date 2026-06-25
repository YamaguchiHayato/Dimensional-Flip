#include "stdafx.h"

#include "InGameBuildHelper.h"

namespace nsApp
{
    void InGameBuildHelper::Initialize()
    {
        buildFunctions_.clear();
        currentBuildIndex_ = 0;
        isFinished_ = false;
        isLoadSuccess_ = true;

        InitializeBuildFunctions();
    }


    void InGameBuildHelper::InitializeBuildFunctions()
    {
        buildFunctions_.clear();

        // 現在は TSV 読み込みのみ。将来ここにステップを追加する。
        buildFunctions_.push_back([this]() { BuildParameters(); });
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
