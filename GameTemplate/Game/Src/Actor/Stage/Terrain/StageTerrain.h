#pragma once

/**
 * @file   StageTerrain.h
 * @brief  ステージ地形（tkm モデル + 物理）の共通初期化処理。
 */

#include "Src/Parameter/Stage/StageMasterRecord.h"

namespace nsTerrain
{
    /**
     * @brief ステージマスターデータに基づき、地形モデルと物理を初期化する。
     * @param[out] stageRender  ステージのモデルレンダラー。
     * @param[out] stagePhysics ステージの静的物理オブジェクト。
     * @param[out] stagePos     ステージのワールド座標（マスタの position を反映）。
     * @param[out] initPos      初期配置座標（stagePos と同値を設定）。
     * @param[in]  master       StageMaster.tsv から取得したステージ定義。
     * @return 初期化に成功した場合は true。
     */
    bool InitializeStageTerrain(ModelRender& stageRender, PhysicsStaticObject& stagePhysics, Vector3& stagePos,Vector3& initPos, const nsApp::nsSystem::StageMasterRecord& master);
} // namespace nsTerrain
