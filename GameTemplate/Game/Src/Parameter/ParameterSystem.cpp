#include "stdafx.h"
#include "ParameterSystem.h"
#include "Src/Parameter/Player/PlayerAirParameterTable.h"
#include "Src/Parameter/Player/PlayerMoveParameterTable.h"
#include "Src/Parameter/Player/PlayerPhysicsParameterTable.h"
#include "Src/Parameter/Player/PlayerStatusParameterTable.h"



namespace nsApp
{
    namespace nsSystem
    {
        bool ParameterSystem::LoadAll()
        {
            /* 全てのパラメータテーブルを読み込む。*/
            return LoadAllParameterTables();
        }


        bool ParameterSystem::LoadAllParameterTables()
        {
            using namespace nsSystem;

            /* 成功フラグを初期化。*/
            isSuccess_ = true;

            /* StatusTSV。*/
            isSuccess_ &= PlayerStatusParameterTable::LoadTSVFile(MakeParameterPath("PlayerStatusParameter.tsv").c_str());

            /* MoveTSV。*/
            isSuccess_ &= PlayerMoveParameterTable::LoadTSVFile(MakeParameterPath("PlayerMoveParameter.tsv").c_str());

            /* AirTSV。*/
            isSuccess_ &= PlayerAirParameterTable::LoadTSVFile(MakeParameterPath("PlayerAirParameter.tsv").c_str());

            /* PhysicsTSV。*/
            isSuccess_ &= PlayerPhysicsParameterTable::LoadTSVFile(MakeParameterPath("PlayerPhysicsParameter.tsv").c_str());

            /* ここに新しいパラメータテーブルの読み込みを追加する場合は、上記のように isSuccess_ に結果を加算する。*/
            return isSuccess_;
        }


        std::string ParameterSystem::MakeParameterPath(const char* fileName) const
        {
            /* パラメーターファイルのディレクトリパスとファイル名を結合して、完全なファイルパスを生成する。*/
            return parameterDirectory_ + fileName;
        }
    }
}
