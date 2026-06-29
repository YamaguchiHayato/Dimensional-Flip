#include "stdafx.h"

#include "Src/Utilty/TSVTable.h"
#include "StageSpawnTable.h"
#include "Src/Parameter/ParameterSystemTableBase.h"  

namespace
{
    /**
     * @brief ステージごとの配置リストを保持するテーブルを取得する。
     * @return ステージごとの配置リストテーブル。
     */
    std::unordered_map<std::string, std::vector<nsApp::nsSystem::StageSpawnRecord>>& GetTable()
    {
        static std::unordered_map<std::string, std::vector<nsApp::nsSystem::StageSpawnRecord>> table;
        return table;
    }

    
    static const std::vector<nsApp::nsSystem::StageSpawnRecord> kEmpty;
} 

namespace nsApp
{
    namespace nsSystem
    {
        bool StageSpawnTable::LoadTSVFile(const char* filePath)
        {
            /* テーブル変数を宣言。*/
            TSVTable tsv;

            /* テーブルを読み込む。*/
            if (!tsv.LoadTSVFile(filePath))
                return false;

            /* テーブルを取得。*/
            auto& table = GetTable();
            table.clear();

            /* テーブルの数だけ処理をループ。*/
            for (int i = 0; i < tsv.GetRowCount(); ++i)
            {
                /* テーブルを作成。*/
                StageSpawnRecord rec = CreateFromRow(tsv, i);

                /* TSV の行からレコードを作成。*/
                if (rec.stageId.empty() || rec.objectType.empty())
                    continue;

                /* リストに追加。*/
                table[rec.stageId].push_back(std::move(rec));
            }
            return !table.empty();
        }


        const std::vector<StageSpawnRecord>& StageSpawnTable::GetByStageID(StageID stageId)
        {
            const char* configId = StageIDToConfigId(stageId);

            /* StageID を ConfigId に変換。*/
            if (configId == nullptr || configId[0] == '\0')
                return kEmpty;

            /* テーブルを取得。*/
            const auto& table = GetTable();

            /* テーブルからステージ ID に対応する配置リストを取得。*/
            auto it = table.find(configId);

            /* ステージ ID に対応する配置リストを検索。*/
            if (it != table.end())
                return it->second;

            /* 配置リストを返す。*/
            return kEmpty;
        }


        const char* StageSpawnTable::StageIDToConfigId(StageID stageId)
        {
            /* 文字列の結果に応じたステージ名を返す。*/
            switch (stageId)
            {
            case StageID::sTutorialStage:
                return "Tutorial";
            case StageID::sStage1:
                return "Stage1";
            case StageID::sStage2:
                return "Stage2";
            case StageID::sStage3:
                return "Stage3";
            case StageID::sStageEX:
                return "StageEX";
            default:
                return "";
            }
        }


        StageSpawnRecord StageSpawnTable::CreateFromRow(const TSVTable& table, int rowIndex)
        {
            /* TSV の行からレコードを作成する。*/
            const ParameterRowReader row{table, rowIndex};

            /* レコードを作成。*/
            StageSpawnRecord rec;
            rec.stageId = table.GetString(rowIndex, "StageId", "");
            rec.objectType = table.GetString(rowIndex, "ObjectType", "");
            rec.posX = row.F("PosX", 0.0f);
            rec.posY = row.F("PosY", 0.0f);
            rec.posZ = row.F("PosZ", 0.0f);
            rec.param0 = row.F("Param0", 0.0f);
            rec.param1 = row.F("Param1", 0.0f);
            rec.param2 = row.F("Param2", 0.0f);
            rec.groupId = table.GetString(rowIndex, "GroupId", "");
            return rec;
        }
    } // namespace nsSystem
} // namespace nsApp
