#pragma once
#include <vector>
#include "Src/Utilty/TSVTable.h"
#include "Src/Parameter/Stage/ScrollBackGroundLayerParameter.h"
#include "Src/Actor/Stage/IStage.h"

namespace nsApp
{
    namespace nsSystem
    {
        using namespace nsStage::nsScrollBackGround;

        class ScrollBackGroundLayerTable
        {
        public:
            /**
             * @brief TSV ファイルを読み込む。
             * @param filePath TSV ファイルパス。
             * @return 読み込み成功時は true、失敗時は false。
             */
            static bool LoadTSVFile(const char* filePath);

            /**
             * @brief 指定したステージ ID に対応するスクロール背景レイヤー定義を取得する。
             * @param stageId ステージ ID（例: "Tutorial"、"NormalStage1"）。
             * @return 対応するスクロール背景レイヤー定義の配列。見つからない場合は空の配列を返す。
             */
            static const std::vector<ScrollBackGroundLayerParameter>& GetByStageId(const char* stageId);

            /**
             * @brief 指定したステージ ID に対応するスクロール背景レイヤー定義を取得する。
             * @param stageId ステージ ID（例: StageID::Tutorial、StageID::NormalStage1）。
             * @return
             */
            static const std::vector<ScrollBackGroundLayerParameter>& GetByStageID(StageID stageId);


        private:
            /**
             * @brief 指定したレイヤー ID 文字列を ScrollLayerKind に変換する。
             * @param layerId レイヤー ID 文字列（例: "Sky"、"Mountain"、"Ground"）。
             * @return 対応する ScrollLayerKind。見つからない場合は ScrollLayerKind::Sky を返す。
             */
            static ScrollLayerKind ParseLayerId(const std::string& layerId);

            /**
             * @brief 指定したステージ ID 文字列を ScrollBackGroundLayerParameter の stageId に変換する。
             * @param stageId StageID 列挙型（例: StageID::Tutorial、StageID::NormalStage1）。
             * @return 対応するステージ ID 文字列。見つからない場合は空文字列を返す。
             */
            static const char* StageIDToConfigId(StageID stageId);

            /**
             * @brief TSV テーブルの行から ScrollBackGroundLayerParameter を作成する。
             * @param table TSV テーブル。
             * @param rowIndex 行インデックス。
             * @return
             */
            static ScrollBackGroundLayerParameter CreateFromRow(const nsApp::TSVTable& table, int rowIndex);


        private:
            static TSVTable tsv_; //! TSV テーブルのインスタンス。
            static const std::vector<ScrollBackGroundLayerParameter> kEmpty_; //! 空の配列。

        };
    }
}
