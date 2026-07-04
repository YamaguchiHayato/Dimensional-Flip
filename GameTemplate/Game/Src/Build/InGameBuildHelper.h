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
#include "Src/Presentation/Data/BossHubData.h"


namespace nsApp
{
    namespace nsUI
    {
        class BossHudScreenHost;
    }

    class InGameBuildHelper
    {
    public:
        /* コンストラクタとデストラクタ。*/
        InGameBuildHelper() = default;
        virtual ~InGameBuildHelper() = default;


    public:
        /**
         * @brief 初期化を開始する。
         * @param stageID ステージ ID。
         */
        void Initialize(nsStage::StageID stageID);

        /**
         * @brief 初期化の次のステップを実行する。
         */
        void Update() { ExecuteNextBuildFunction(); }

        /**
         * @brief 初期化が完了したかどうかを取得する。
         * @return 初期化が完了していれば true、そうでなければ false。
         */
        bool IsFinished() const { return isFinished_; }

        /**
         * @brief 初期化が成功したかどうかを取得する。
         * @return 初期化が成功していれば true、そうでなければ false。
         */
        bool IsLoadSuccess() const { return isLoadSuccess_; }

        /**
         * @brief 初期化の進捗を取得する。
         * @return 進捗（0.0f から 1.0f の範囲）。
         */
        float GetProgress() const;

        /**
         * @brief 初期化が完了した後に生成された背景を取得する。
         * @return 生成された背景。初期化が完了していない場合は nullptr。
         */
        nsStage::nsBackGround::IBackGround* GetBackGround() const { return pBackGround_; }

        /**
         * @brief ステージ ID に基づいて背景を生成する。
         * @param stageID ステージ ID。
         * @return 生成された背景。失敗した場合は nullptr。
         */
        nsStage::nsBackGround::IBackGround* CreateBackGround(nsStage::StageID stageID);

        /**
         * @brief ボス HUD スクリーンホストを取得する。
         * @return ボス HUD スクリーンホスト。初期化が完了していない場合は nullptr。
         */
        nsUI::BossHudScreenHost* GetBossHudScreenHost() const { return pBossHudHost_; }

        /**
         * @brief ボス HUD データを取得する。
         * @return ボス HUD データ。初期化が完了していない場合は nullptr。
         */
        nsPresentation::BossHudData* GetBossHudData() { return &bossHudData_; }


    private:
        using BuildFunction = std::function<void()>; //! ビルド関数の型定義。

        /**
         * @brief ビルド関数を初期化する。
         */
        void InitializeBuildFunctions();
        void ExecuteNextBuildFunction();
        void BuildParameters();
        void BuildBackGroundStep();
        void FinishBuild();
        void BuildBossHudUi();


    private:
        nsSystem::ParameterSystem parameterSystem_;
        std::vector<BuildFunction> buildFunctions_;
        nsStage::nsBackGround::IBackGround* pBackGround_ = nullptr;
        nsStage::StageID stageID_ = nsStage::StageID::sInvalid;
        nsUI::BossHudScreenHost* pBossHudHost_ = nullptr;
        nsPresentation::BossHudData bossHudData_;
        bool isFinished_ = false;
        bool isLoadSuccess_ = true;
        int currentBuildIndex_ = 0;
    };
} // namespace nsApp
