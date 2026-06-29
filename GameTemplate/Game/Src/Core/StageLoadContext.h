#pragma once

#include <mutex>
#include <string>
#include <vector>

#include "Src/Actor/Stage/StageID.h"

namespace nsApp
{
    namespace nsStage
    {
        /**
         * @class StageLoadContext
         * @brief ステージ遷移のワーカー先読み結果をメインスレッドへ渡す。
         * @note  J-2: ファイル読み込みのみ。J-3: StageTerrain がバッファを消費する。
         */
        class StageLoadContext
        {
        public:
            /** @brief 遷移開始時に呼ぶ。前回のバッファを破棄する。 */
            static void Reset();

            /**
             * @brief ワーカースレッドで地形モデルを先読みする。
             * @param stageID 遷移先ステージ。
             */
            static void PrepareOnWorker(StageID stageID);

            /** @brief 先読みが成功したか。 */
            static bool IsReady();

            /** @brief 先読みしたモデルパス（J-3 用）。 */
            static std::string GetModelPath();

            /** @brief 先読みしたバイナリ（J-3 用。J-2 では未使用）。 */
            static const std::vector<char>& GetModelBuffer();

        private:
            static std::mutex mutex_;
            static std::vector<char> modelBuffer_;
            static std::string modelPath_;
            static bool isReady_;
        };
    } // namespace nsStage
} // namespace nsApp
