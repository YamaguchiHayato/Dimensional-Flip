#pragma once

/**
 * @file   InGameBuildHelper.h
 * @brief  InGame 起動時の段階的初期化を管理する。
 * @detail 1フレームに1つの Build ステップを実行する。
 *         ParameterSystem の TSV 読み込み、ステージ背景の生成などを行う。
 */

#include <functional>
#include <vector>

#include "IBackGround.h"
#include "Src/Actor/Stage/IStage.h"
#include "Src/Parameter/ParameterSystem.h"

namespace nsApp
{
    /**
     * @class InGameBuildHelper
     * @brief InGame 生成補助クラス。
     */
    class InGameBuildHelper
    {
    public:
        InGameBuildHelper() = default;
        virtual ~InGameBuildHelper() = default;

        /**
         * @brief 初期化処理。
         * @param stageID 生成対象ステージ ID。
         * @note  Build ステップの登録と進行状態のリセットを行う。
         */
        void Initialize(StageID stageID);

        /**
         * @brief 更新処理。
         * @note  1フレームに1つの Build ステップを実行する。
         */
        void Update() { ExecuteNextBuildFunction(); }

        /**
         * @brief 全 Build ステップが完了したか。
         * @return 完了していれば true。
         */
        bool IsFinished() const { return isFinished_; }

        /**
         * @brief パラメータ読み込みに成功したか。
         * @return 成功していれば true。
         */
        bool IsLoadSuccess() const { return isLoadSuccess_; }

        /**
         * @brief 進捗率を取得する。
         * @return 0.0f 〜 1.0f。
         */
        float GetProgress() const;

        /**
         * @brief 起動時 Build で生成した背景を取得する。
         * @return 背景オブジェクト。未生成なら nullptr。
         */
        nsStage::nsBackGround::IBackGround* GetBackGround() const { return pBackGround_; }

        /**
         * @brief ステージ ID に応じた背景 GameObject を生成する。
         * @param stageID ステージ ID。
         * @return 生成した背景。対象外ステージは nullptr。
         * @note  呼び出し側で DeleteGO の管理を行う。
         */
        nsStage::nsBackGround::IBackGround* CreateBackGround(StageID stageID);

    private:
        using BuildFunction = std::function<void()>;

        /** @brief Build ステップを登録する。 */
        void InitializeBuildFunctions();

        /** @brief 次の Build ステップを1つ実行する。 */
        void ExecuteNextBuildFunction();

        /** @brief TSV パラメータを ParameterSystem 経由で読み込む。 */
        void BuildParameters();

        /** @brief ステージ背景を生成する Build ステップ。 */
        void BuildBackGroundStep();

        /** @brief Build 完了処理。 */
        void FinishBuild();

    private:
        nsSystem::ParameterSystem parameterSystem_; //! パラメータ読み込み窓口。

        std::vector<BuildFunction> buildFunctions_; //! 登録済み Build ステップ。

        nsStage::nsBackGround::IBackGround* pBackGround_ = nullptr; //! 起動時 Build で生成した背景。

        StageID stageID_ = StageID::sInvalid; //! 生成対象ステージ ID。

        bool isFinished_ = false;   //! 全ステップ完了フラグ。
        bool isLoadSuccess_ = true; //! パラメータ読み込み成功フラグ。

        int currentBuildIndex_ = 0; //! 現在実行中のステップ番号。
    };
} // namespace nsApp
