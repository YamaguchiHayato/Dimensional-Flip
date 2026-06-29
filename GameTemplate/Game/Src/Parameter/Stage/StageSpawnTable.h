#pragma once

#include <vector>

#include "Src/Actor/Stage/StageID.h"
#include "Src/Parameter/Stage/StageSpawnRecord.h"
#include "Src/Utilty/TSVTable.h"

namespace nsApp
{
    namespace nsSystem
    {
        class StageSpawnTable
        {
        public:
            /**
             * @brief TSVファイルを読み込む。
             * @param filePath ファイルパス。
             * @return 読み込みに成功した場合はtrue、失敗した場合はfalse。
             */
            static bool LoadTSVFile(const char* filePath);

            /**
             * @brief ステージIDに対応するStageSpawnRecordのリストを取得する。
             * @param stageId ステージID。 
             * @return ステージIDに対応するStageSpawnRecordのリスト。存在しない場合は空のリストを返す。
             */
            static const std::vector<StageSpawnRecord>& GetByStageID(nsStage::StageID stageId);


        private:
            /**
             * @brief StageIDを文字列に変換する。
             * @param stageId ステージID。
             * @return StageIDに対応する文字列。存在しない場合はnullptrを返す。
             */
            static const char* StageIDToConfigId(nsStage::StageID stageId);

            /**
             * @brief StageIDを文字列に変換する。
             * @param table TSVTableの参照。
             * @param rowIndex 行インデックス。
             * @return StageIDに対応する文字列。存在しない場合はnullptrを返す。
             */
            static StageSpawnRecord CreateFromRow(const TSVTable& table, int rowIndex);
        };
    } // namespace nsSystem
} // namespace nsApp
