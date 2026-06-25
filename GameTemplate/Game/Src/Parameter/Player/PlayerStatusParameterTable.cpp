#include "stdafx.h"

#include "PlayerStatusParameterTable.h"
#include "Src/Utilty/TSVTable.h"

namespace nsApp
{
    namespace nsSystem
    {
        namespace
        {
            PlayerStatusParameter CreateFromRow(const TSVTable& table, int rowIndex)
            {
                /* TSVTable から PlayerStatusParameter を作成する。*/
                const ParameterRowReader row{table, rowIndex};

                /* デフォルト値を指定してパラメータを読み取る。*/
                return PlayerStatusParameter{
                    row.I("MaxHP", 9),
                    row.F("AttackPower", 1.0f),
                    row.F("InvincibleTime", 1.0f),
                    row.I("FallRespawnDamage", 1),
                };
            }
        } 


        bool PlayerStatusParameterTable::LoadTSVFile(const char* filePath)
        {
            /* TSV ファイルから PlayerStatusParameterTable を読み込む。*/
            return Base::Load(
                filePath, "Id", [](const std::string& key) { return key; },
                [](const TSVTable& table, int rowIndex, const std::string&) { return CreateFromRow(table, rowIndex); });
        }


        const PlayerStatusParameter& PlayerStatusParameterTable::Get()
        {
            /* デフォルトの PlayerStatusParameter を取得する。*/
            static const std::string defaultKey = "Default";

            /* デフォルトの PlayerStatusParameter が存在しない場合は、エラーを出力する。*/
            return Base::Get(defaultKey);
        }
    } // namespace nsSystem
} // namespace nsApp
