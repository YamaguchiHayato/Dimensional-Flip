#include "stdafx.h"
#include "Stage.h"
#include "Src/Actor/Stage/Terrain/StageTerrain.h"
#include "Src/Parameter/Stage/StageMasterTable.h"


namespace nsApp
{
    namespace nsStage
    {
        Vector3 Stage::GetPlayerStartPos() const
        {
            /* Stageのマスターデータを取得。*/
            const auto& master = nsSystem::StageMasterTable::Get(stageID_);

            /* マスターデータが無効な場合は、デフォルトの開始位置を返す。*/
            if (master.stageID != StageID::sInvalid)
                return master.playerStartPosition;

            /* マスターデータが無効な場合は、デフォルトの開始位置を返す。*/
            return Vector3(0.0f, 20.0f, 0.0f);
        }


        bool Stage::Start()
        {
            /* Stageのマスターデータを取得。*/
            const auto& master = nsSystem::StageMasterTable::Get(stageID_);

            /* マスターデータが無効な場合は、ステージの初期化に失敗したことを示す。*/
            if (master.stageID == StageID::sInvalid)
                return false;

            /* ステージの初期化を行う。*/
            Vector3 initPos = Vector3::Zero;

            return nsTerrain::InitializeStageTerrain(
                stageRender_,
                stagePhysics_,
                stagePos_,
                initPos,
                master);
        }


        void Stage::Update()
        {
            /* ステージの更新処理を行う。*/
            stageRender_.Update();
        }


        void Stage::Render(RenderContext& rc)
        {
            /* ステージの描画処理を行う。*/
            stageRender_.Draw(rc);
        }
    }
}
