#include "stdafx.h"
#include "StageTerrain.h"

namespace nsTerrain
{
    bool InitializeStageTerrain(ModelRender& stageRender, PhysicsStaticObject& stagePhysics, Vector3& stagePos, Vector3& initPos, const nsApp::nsSystem::StageMasterRecord& master)
    {
        /* マスタデータが無効な場合は失敗する。*/
        if (master.stageID == nsApp::nsStage::StageID::sInvalid)
            return false;

        /* モデルパスが空の場合は失敗する。*/
        if (master.modelPath.empty())
            return false;

        /* ステージ配置座標をマスタから反映する。*/
        stagePos = master.position;
        initPos = stagePos;

        /* 地形モデルをロードする。*/
        stageRender.Init(master.modelPath.c_str());

        /* スケール・位置・回転を適用する。*/
        stageRender.SetScale(master.scale);
        stageRender.SetPosition(stagePos);

        if (master.rotY != 0.0f)
        {
            Quaternion rot;
            rot.SetRotationDegY(master.rotY);
            stageRender.SetRotation(rot);
        }

        /* ワールド行列を更新してから物理を生成する（StageEX と同順）。*/
        stageRender.Update();
        stagePhysics.CreateFromModel(stageRender.GetModel(), stageRender.GetModel().GetWorldMatrix());

        return true;
    }
} // namespace nsTerrain
