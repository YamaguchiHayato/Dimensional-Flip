#include "stdafx.h"

#include "PlayerAirParameterTable.h"
#include "Src/Utilty/TSVTable.h"

namespace nsApp
{
    namespace nsSystem
    {
        namespace
        {
            PlayerAirParameter CreateFromRow(const TSVTable& table, int rowIndex)
            {
                const ParameterRowReader row{table, rowIndex};
                return PlayerAirParameter{
                    row.F("JumpPower", 150.0f),      row.F("BounceJumpPower", 100.0f), row.F("GravityBase", 8.0f),
                    row.F("JumpGravityScale", 1.2f), row.F("JumpCutScale", 2.0f),      row.F("MaxFallSpeed", -200.0f),
                    row.F("AirMoveRateJump", 0.7f),  row.F("AirMoveRateFall", 0.4f),   row.F("BounceCooldown", 0.2f),
                };
            }
        } 


        bool PlayerAirParameterTable::LoadTSVFile(const char* filePath)
        {
            /* TSVファイルを読み込み、テーブルを作成する。*/
            return Base::Load(
                filePath, "Id", [](const std::string& key) { return key; },
                [](const TSVTable& table, int rowIndex, const std::string&) { return CreateFromRow(table, rowIndex); });
        }


        const PlayerAirParameter& PlayerAirParameterTable::Get()
        {
            /* デフォルトのパラメータを返す。*/
            static const std::string defaultKey = "Default";

            /* デフォルトのパラメータが存在しない場合は、最初の行を返す。*/
            return Base::Get(defaultKey);
        }
    } // namespace nsSystem
} // namespace nsApp
