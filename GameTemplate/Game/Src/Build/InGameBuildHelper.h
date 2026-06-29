#pragma once

/**
 * @file   InGameBuildHelper.h
 * @brief  InGame 起動時の段階的初期化を管理する。
 */

#include <functional>
#include <vector>

#include "IBackGround.h"
#include "Src/Actor/Stage/StageID.h"
#include "Src/Parameter/ParameterSystem.h"

namespace nsApp
{
    class InGameBuildHelper
    {
    public:
        /* コンストラクタとデストラクタ。*/
        InGameBuildHelper() = default;
        virtual ~InGameBuildHelper() = default;

        /** @brief Build ステップを初期化する。 */
        void Initialize(nsStage::StageID stageID);

        /** @brief 1フレームに1 Build ステップ実行。 */
        void Update() { ExecuteNextBuildFunction(); }

        bool IsFinished() const { return isFinished_; }
        bool IsLoadSuccess() const { return isLoadSuccess_; }
        float GetProgress() const;

        nsStage::nsBackGround::IBackGround* GetBackGround() const { return pBackGround_; }

        /** @brief ステージ ID に応じた背景を生成する。 */
        nsStage::nsBackGround::IBackGround* CreateBackGround(nsStage::StageID stageID);


    private:
        using BuildFunction = std::function<void()>;

        void InitializeBuildFunctions();
        void ExecuteNextBuildFunction();
        void BuildParameters();
        void BuildBackGroundStep();
        void FinishBuild();


    private:
        nsSystem::ParameterSystem parameterSystem_;
        std::vector<BuildFunction> buildFunctions_;
        nsStage::nsBackGround::IBackGround* pBackGround_ = nullptr;
        nsStage::StageID stageID_ = nsStage::StageID::sInvalid;
        bool isFinished_ = false;
        bool isLoadSuccess_ = true;
        int currentBuildIndex_ = 0;
    };
} // namespace nsApp
