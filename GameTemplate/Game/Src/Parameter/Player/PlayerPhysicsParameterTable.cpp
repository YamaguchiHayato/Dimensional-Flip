#include "stdafx.h"

#include "PlayerPhysicsParameterTable.h"
#include "Src/Utilty/TSVTable.h"

namespace nsApp
{
    namespace nsSystem
    {
        namespace
        {
            PlayerPhysicsParameter CreateFromRow(const TSVTable& table, int rowIndex)
            {
                /* TSV の行から PlayerPhysicsParameter を作成する。*/
                const ParameterRowReader row{table, rowIndex};
                return PlayerPhysicsParameter{
                    row.F("ColliderRadius", 2.0f),
                    row.F("ColliderHeight", 1.0f),
                    row.F("FixedDeltaTime", 1.0f / 150.0f),
                    row.F("DeadLineY", -100.0f),
                };
            }
        }


        bool PlayerPhysicsParameterTable::LoadTSVFile(const char* filePath)
        {
            /* TSV ファイルを読み込み、テーブルを構築する。*/
            return Base::Load(
                filePath, "Id", [](const std::string& key) { return key; },
                [](const TSVTable& table, int rowIndex, const std::string&) { return CreateFromRow(table, rowIndex); });
        }


        const PlayerPhysicsParameter& PlayerPhysicsParameterTable::Get()
        {
            /* デフォルトの値を返す。*/
            static const std::string defaultKey = "Default";

            /* デフォルトの値が存在しない場合は、最初の行を返す。*/
            return Base::Get(defaultKey);
        }
    } // namespace nsSystem
} // namespace nsApp
