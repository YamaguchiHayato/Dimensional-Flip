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
#include "Src/Presentation/Data/GameplayHudData.h"


namespace nsApp
{
    namespace nsUI
    {
        class BossHudScreenHost;
        class GameplayHudScreenHost;
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

        /**
         * @brief ゲームプレイ HUD スクリーンホストを取得する。
         * @return ゲームプレイ HUD スクリーンホスト。初期化が完了していない場合は nullptr。
         */
        nsUI::GameplayHudScreenHost* GetGameplayHudScreenHost() const { return pGameplayHudScreenHost_; }

        /**
         * @brief ゲームプレイ HUD データを取得する。
         * @return ゲームプレイ HUD データ。初期化が完了していない場合は nullptr。
         */
        nsPresentation::GameplayHudData* GetGameplayHudData() { return &gameplayHudData_; }

        /**
         * @brief ゲームプレイ HUD データとスクリーンを接続する。
         */
        void ConnectGameplayHudData();

        /**
         * @brief ゲームプレイ HUD を破棄する。
         */
        void DestroyGameplayHud();


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
        void BuildGameplayHudUiStep();


    private:
        nsSystem::ParameterSystem parameterSystem_; //! パラメータシステム。
        std::vector<BuildFunction> buildFunctions_; //! < ビルド関数のリスト。
        nsStage::nsBackGround::IBackGround* pBackGround_ = nullptr; //! < 生成された背景。
        nsStage::StageID stageID_ = nsStage::StageID::sInvalid;     //! < ステージ ID。
        nsUI::BossHudScreenHost* pBossHudHost_ = nullptr;           //! < 生成されたボス HUD スクリーンホスト。
        nsPresentation::BossHudData bossHudData_;                   //! < ボス HUD データ。
        nsPresentation::GameplayHudData gameplayHudData_;           //! < ゲームプレイ HUD データ。
        nsUI::GameplayHudScreenHost* pGameplayHudScreenHost_ = nullptr;//! < 生成されたゲームプレイ HUD スクリーンホスト。

        bool isFinished_ = false; //! < 初期化が完了したかどうか。
        bool isLoadSuccess_ = true; //! < 初期化が成功したかどうか。
        int currentBuildIndex_ = 0; //! < 現在のビルド関数のインデックス。
    };
} // namespace nsApp
