#include "stdafx.h"

#include "PlayerMoveParameterTable.h"
#include "Src/Utilty/TSVTable.h"

namespace nsApp
{
    namespace nsSystem
    {
        PlayerMoveParameter CreateFromRow(const TSVTable& table, int rowIndex)
        {
           const ParameterRowReader row{table, rowIndex};
           return PlayerMoveParameter{
               row.F("WalkSpeed", 100.0f),
           };
        }


        bool PlayerMoveParameterTable::LoadTSVFile(const char* filePath)
        {
            /* TSVファイルを読み込み、テーブルを作成する。*/
            return Base::Load(
                filePath, "Id", [](const std::string& key) { return key; },
                [](const TSVTable& table, int rowIndex, const std::string&) { return CreateFromRow(table, rowIndex); });
        }


        const PlayerMoveParameter& PlayerMoveParameterTable::Get()
        {
            /* デフォルトのパラメータを返す。*/
            static const std::string defaultKey = "Default";

            /* デフォルトのパラメータを返す。*/
            return Base::Get(defaultKey);
        }
    } // namespace nsSystem
} // namespace nsApp
