#include "stdafx.h"

#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Enemy/EnemyFactory.h"
#include "Src/Actor/Character/Enemy/Thwomp.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Stage/Gimmick/StageGimmick/Star.h"
#include "Src/Camera/Dimensiontrigger.h"
#include "Src/Core/BossUIManager.h"
#include "Src/Parameter/Stage/StageSpawnTable.h"
#include "Src/Production/CutIn/CutInView.h"
#include "StageObjectSpawner.h"
#include "Src/Actor/Stage/Gimmick/StageGimmick/RotationFool.h"

namespace nsApp
{
    namespace nsStage
    {
        void StageObjectSpawner::Spawn(StageID stageId)
        {
            /* クリア。*/
            Clear();

            /* ステージIDに紐づくレコードを取得して、1件ずつ生成。*/
            const auto& records = nsSystem::StageSpawnTable::GetByStageID(stageId);
            for (const auto& rec : records)
                SpawnOne(rec);
        }


        void StageObjectSpawner::Clear()
        {
            /* 生成済みのオブジェクトを削除。*/
            for (auto* obj : spawnedObjects_)
                DeleteGO(obj);

            /* 生成済みオブジェクトのリストをクリア。*/
            spawnedObjects_.clear();
        }


        void StageObjectSpawner::Unregister(IGameObject* obj)
        {
            /* spawnedObjects_ から obj を削除する。*/
            auto it = std::find(spawnedObjects_.begin(), spawnedObjects_.end(), obj);
            if (it != spawnedObjects_.end())
                spawnedObjects_.erase(it);
        }


        void StageObjectSpawner::SpawnOne(const nsSystem::StageSpawnRecord& rec)
        {
            /* 位置を設定。*/
            const Vector3 pos(rec.posX, rec.posY, rec.posZ);

            /* オブジェクトタイプに応じて生成。*/
            if (rec.objectType == "Star")
            {
                auto* star = NewGO<Star>(0, "star");
                star->SetStarPosition(pos);
                spawnedObjects_.push_back(star);
            }
            /* DimensionTrigger の生成。*/
            else if (rec.objectType == "DimensionTrigger")
            {
                auto* trigger = NewGO<DimensionTrigger>(0, "dimensiontrigger");
                trigger->SetTriggerPos(pos);
                spawnedObjects_.push_back(trigger);
            }
            /* 敵の生成。*/
            else if (rec.objectType == "NormalEnemy")
            {
                auto* player = FindGO<Player>("player");
                auto enemies = app::enemy::EnemyFactory::CreateEnemy(EnemyType::type_Normal, pos, player);
                for (auto* enemy : enemies)
                {
                    if (enemy)
                        spawnedObjects_.push_back(enemy);
                }
            }

            /* Thwomp の生成。*/
            else if (rec.objectType == "Thwomp")
            {
                auto* player = FindGO<Player>("player");
                auto enemies = app::enemy::EnemyFactory::CreateEnemy(EnemyType::type_Thwomp, pos, player);

                for (auto* enemy : enemies)
                    if (enemy)
                        spawnedObjects_.push_back(enemy);
            }

            /* Boss の生成。*/
            else if (rec.objectType == "Boss")
            {
                // 名前 "boss" 固定 → SideCameraStrategy がボスカメラに切替
                auto* boss = NewGO<app::enemy::Boss>(0, "boss");
                boss->SetPos(pos);
                spawnedObjects_.push_back(boss);
            }

            /* CutIn の生成。*/
            else if (rec.objectType == "CutIn")
            {
                auto* cutIn = NewGO<CutInView>(0, "CutInView");
                cutIn->SetLifeDuration(rec.param0 > 0.0f ? rec.param0 : 4.0f);
                spawnedObjects_.push_back(cutIn);
            }
        }
    } // namespace nsStage
} // namespace nsApp
