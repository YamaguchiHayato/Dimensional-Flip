#pragma once

#include "Src/Parameter/ParameterSystemTableBase.h"
#include "StageMasterRecord.h"

namespace nsApp
{
    namespace nsSystem
    {
        class StageMasterTable : private ParameterTableBase<nsStage::StageID, StageMasterRecord>
        {
            /**
             * @brief ParameterTableBaseのエイリアス。
             */
            using Base = ParameterTableBase<nsStage::StageID, StageMasterRecord>;

        public:
            /**
             * @brief TSVファイルを読み込む。
             * @param filePath ファイルパス。
             * @return 読み込みに成功した場合はtrue、失敗した場合はfalse。
             */
            static bool LoadTSVFile(const char* filePath);

            /**
             * @brief ステージIDに対応するステージマスターレコードを取得する。
             * @param stageID ステージID。
             * @return ステージマスターレコード。
             */
            static const StageMasterRecord& Get(nsStage::StageID stageID);
        };
    } // namespace nsSystem
} // namespace nsApp
