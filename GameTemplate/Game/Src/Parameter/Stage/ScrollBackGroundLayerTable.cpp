#include "stdafx.h"

#include <algorithm>
#include <unordered_map>

#include "ScrollBackGroundLayerTable.h"
#include "Src/Parameter/ParameterSystemTableBase.h"
#include "Src/Utilty/TSVTable.h"

namespace
{
    using nsApp::nsStage::nsScrollBackGround::ScrollBackGroundLayerParameter;
    using nsApp::nsStage::nsScrollBackGround::ScrollLayerKind;
    using nsApp::nsSystem::ParameterRowReader;

     ScrollLayerKind ParseLayerId(const std::string& layerId)
    {
        if (layerId == "Mountain")
            return ScrollLayerKind::Mountain;
        if (layerId == "Ground")
            return ScrollLayerKind::Ground;
        return ScrollLayerKind::Sky;
    }


    /**
     * @brief TSV テーブルの行を読み込むためのヘルパークラス。
     * @return テーブルの値。
     */
    std::unordered_map<std::string, std::vector<ScrollBackGroundLayerParameter>>& GetStageTable()
    {
        /* テーブルの作成。*/
        static std::unordered_map<std::string, std::vector<ScrollBackGroundLayerParameter>> table;
        return table;
    }


    void SortLayersByRenderOrder(std::vector<ScrollBackGroundLayerParameter>& layers)
    {
        std::sort(layers.begin(), layers.end(),
                  [](const ScrollBackGroundLayerParameter& a, const ScrollBackGroundLayerParameter& b)
                  { return a.renderOrder < b.renderOrder; });
    }
} // namespace

namespace nsApp
{
    namespace nsSystem
    {
        /* Static変数の実態を宣言する。*/
        TSVTable ScrollBackGroundLayerTable::tsv_;
        const std::vector<ScrollBackGroundLayerParameter> ScrollBackGroundLayerTable::kEmpty_;
    

        bool ScrollBackGroundLayerTable::LoadTSVFile(const char* filePath)
        {
            /* テーブルを取得する。*/
            auto& table = GetStageTable();
            table.clear();

            /* ファイルがない場合、エラーメッセージをだす。*/
            if (!tsv_.LoadTSVFile(filePath))
            {
                OutputDebugStringA( ("ScrollBackGroundLayerTable::LoadTSVFile failed: " + tsv_.GetLastError() + "\n").c_str());
                return false;
            }

            /* TSVファイルの行数を取得する。*/
            const int rowCount = tsv_.GetRowCount();
            for (int rowIndex = 0; rowIndex < rowCount; ++rowIndex)
            {
                /* TSVファイルの行数を取得。*/
                ScrollBackGroundLayerParameter param = CreateFromRow(tsv_, rowIndex);

                /* stageId が空の場合はスキップする。*/
                if (param.stageId.empty())
                    continue;

                /* stageId が空の場合はスキップする。*/
                table[param.stageId].push_back(std::move(param));
            }

            /* 各ステージのレイヤーを描画順にソートする。*/
            for (auto& pair : table)
                SortLayersByRenderOrder(pair.second);

            /* empty を返す。*/
            return !table.empty();
        }


        const std::vector<ScrollBackGroundLayerParameter>& ScrollBackGroundLayerTable::GetByStageId(const char* stageId)
        {
            /* 指定したステージ ID に対応するスクロール背景レイヤー定義を取得する。*/ 
            if (stageId == nullptr || stageId[0] == '\0')
                return kEmpty_;

            /* TSVを取得。*/
            const auto& table = GetStageTable();

            /* TSVを探索。*/
            const auto it = table.find(stageId);
            if (it != table.end())
                return it->second;

            return kEmpty_;
        }


        const std::vector<ScrollBackGroundLayerParameter>& ScrollBackGroundLayerTable::GetByStageID(StageID stageId)
        {
           const char* configId = StageIDToConfigId(stageId);
            if (configId == nullptr || configId[0] == '\0')
                return GetByStageId("Tutorial"); 

            const auto& layers = GetByStageId(configId);
            if (!layers.empty())
                return layers;

            return GetByStageId("Tutorial"); 
        }


        ScrollLayerKind ScrollBackGroundLayerTable::ParseLayerId(const std::string& layerId)
        {
            /* 文字列に応じた列挙を取得する。*/
            if (layerId == "Mountain")
                return ScrollLayerKind::Mountain;

            if (layerId == "Ground")
                return ScrollLayerKind::Ground;

            return ScrollLayerKind::Sky;
        }


        const char* ScrollBackGroundLayerTable::StageIDToConfigId(StageID stageId)
        {
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
            default:
                return "";
            }
        }


        ScrollBackGroundLayerParameter ScrollBackGroundLayerTable::CreateFromRow(const nsApp::TSVTable& table, int rowIndex)
        {
            /* ParameterRowReader を使用して、行の値を取得する。*/
            const ParameterRowReader row{table, rowIndex};

            /* ScrollBackGroundLayerParameter を作成して、TSV の値を設定する。*/
            ScrollBackGroundLayerParameter param;

            /* TSV の列名に対応する値を取得して、param に設定する。*/
            param.stageId = table.GetString(rowIndex, "StageId", "");

            /* LayerId 列の値を解析して、ScrollLayerKind に変換する。*/
            param.layerKind = ScrollBackGroundLayerTable::ParseLayerId(table.GetString(rowIndex, "LayerId", "Sky"));

            /* TexturePath 列の値を取得する。*/
            param.texturePath = table.GetString(rowIndex, "TexturePath", "");

            /* FallbackTexturePath 列の値を取得する。*/
            param.parallax = row.F("Parallax", 0.0f);

            /* WorldY 列の値を取得する。*/
            param.tileCount = row.I("TileCount", 1);

            /* RenderOrder 列の値を取得する。*/
            param.renderOrder = row.I("RenderOrder", 0);

            /* ScreenY 列の値を取得する。*/
            param.screenHeightRatio = row.F("ScreenHeightRatio", 1.0f);

            /* ScreenCenterY 列の値を取得する。*/
            param.screenCenterY = row.F("ScreenCenterY", 0.0f);

            /* ParallaxPixelScale 列の値を取得する。*/
            param.parallaxPixelScale = row.F("ParallaxPixelScale", 1.0f);

            /* AnchorBottom 列の値を取得する。*/
            param.anchorBottom = row.I("AnchorBottom", 0) != 0;

            /* 取得した値を返す。*/
            return param;
        } 
    } }
