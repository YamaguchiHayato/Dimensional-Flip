#include "stdafx.h"

#include "Src/Actor/Stage/StageID.h"
#include "StageMasterTable.h"

namespace
{
    /** @brief TSV の StageId 文字列を StageID に変換する。 */
    nsApp::nsStage::StageID ParseStageId(const std::string& stageId)
    {
        using nsApp::nsStage::StageID;

        if (stageId == "Tutorial")
            return StageID::sTutorialStage;
        if (stageId == "Stage1")
            return StageID::sStage1;
        if (stageId == "Stage2")
            return StageID::sStage2;
        if (stageId == "Stage3")
            return StageID::sStage3;
        if (stageId == "StageEX")
            return StageID::sStageEX;
        return StageID::sInvalid;
    }
} // namespace

namespace nsApp
{
    namespace nsSystem
    {
        namespace
        {
            StageMasterRecord CreateFromRow(const TSVTable& table, int rowIndex, nsStage::StageID stageID)
            {
                const ParameterRowReader row{table, rowIndex};

                StageMasterRecord record;
                record.stageID = stageID;
                record.modelPath = table.GetString(rowIndex, "ModelPath");
                record.playerStartPosition =
                    Vector3(row.F("StartX", 0.0f), row.F("StartY", 20.0f), row.F("StartZ", 0.0f));
                record.scale = Vector3(row.F("ScaleX", 1.0f), row.F("ScaleY", 1.0f), row.F("ScaleZ", 1.0f));
                record.position = Vector3(row.F("PosX", 0.0f), row.F("PosY", 0.0f), row.F("PosZ", 0.0f));
                record.rotY = row.F("RotY", 0.0f);
                record.backgroundType = table.GetString(rowIndex, "BackgroundType");
                return record;
            }
        }


        bool StageMasterTable::LoadTSVFile(const char* filePath)
        {
            return Base::Load(
                filePath, "StageId", [](const std::string& key) { return ParseStageId(key); },
                [](const TSVTable& table, int rowIndex, nsStage::StageID stageID)
                { return CreateFromRow(table, rowIndex, stageID); });
        }


        const StageMasterRecord& StageMasterTable::Get(nsStage::StageID stageID)
        {
            return Base::Get(stageID);
        }
    } // namespace nsSystem
} // namespace nsApp
