#pragma once

/**
 * @file   InGameBuildHelper.h
 * @brief  InGame 起動時の段階的初期化を管理する。
 * @detail 1フレームに1つの Build ステップを実行する。
 *         現在は ParameterSystem による TSV 読み込みのみ。
 *         将来は BuildPlayer() などを buildFunctions_ に追加する。
 */

#include <functional>
#include <vector>

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
        /* コンストラクタとデストラクタ。*/
        InGameBuildHelper() = default;
        virtual ~InGameBuildHelper() = default;


    public:
        /**
         * @brief 初期化処理。
         * @note  Build ステップの登録と進行状態のリセットを行う。
         */
        void Initialize();

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


    private:
        using BuildFunction = std::function<void()>;

        /** @brief Build ステップを登録する。 */
        void InitializeBuildFunctions();

        /** @brief 次の Build ステップを1つ実行する。 */
        void ExecuteNextBuildFunction();

        /** @brief TSV パラメータを ParameterSystem 経由で読み込む。 */
        void BuildParameters();

        /** @brief Build 完了処理。 */
        void FinishBuild();


    private:
        nsSystem::ParameterSystem parameterSystem_; //! パラメータ読み込み窓口。

        std::vector<BuildFunction> buildFunctions_; //! 登録済み Build ステップ。

        bool isFinished_ = false;                   //! 全ステップ完了フラグ。
        bool isLoadSuccess_ = true;                 //! パラメータ読み込み成功フラグ。

        int currentBuildIndex_ = 0;                 //! 現在実行中のステップ番号。
    };
} // namespace nsApp
