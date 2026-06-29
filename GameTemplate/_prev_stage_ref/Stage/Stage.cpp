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
            const auto& master = nsSystem::StageMasterTable::Get(stageID_);
            if (master.stageID != nsApp::StageID::sInvalid)
                return master.playerStartPosition;

            return Vector3(0.0f, 20.0f, 0.0f);
        }


        bool Stage::Start()
        {
            const auto& master = nsSystem::StageMasterTable::Get(stageID_);
            if (master.stageID == nsApp::StageID::sInvalid)
                return false;

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
            stageRender_.Update();
        }


        void Stage::Render(RenderContext& rc)
        {
            stageRender_.Draw(rc);
        }
    }
}