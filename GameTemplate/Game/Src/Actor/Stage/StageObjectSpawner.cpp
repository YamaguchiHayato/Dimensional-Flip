#include "stdafx.h"

#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Enemy/EnemyFactory.h"
#include "Src/Actor/Character/Enemy/Thwomp.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Stage/Gimmick/StageGimmick/Star.h"
#include "Src/Camera/Dimensiontrigger.h"
#include "Src/Parameter/Stage/StageSpawnTable.h"
#include "Src/Production/CutIn/CutInView.h"
#include "StageObjectSpawner.h"
#include "Src/Core/Game.h"
#include "Src/Actor/Stage/Gimmick/StageGimmick/RotationFool.h"
#include "Src/Presentation/UI/Screens/BossHubScreenHost.h"

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


            /* 通常敵 の生成。*/
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


            /* 回転敵 の生成。*/
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
                /* Boss の生成。*/
                auto* boss = NewGO<app::enemy::Boss>(0, "boss");
                boss->SetPos(pos);
                spawnedObjects_.push_back(boss);

                /* BossHudData へ登録（接続は Host::Start / StageSetup が担当） */
                if (auto* pGame = FindGO<nsApp::nsCore::Game>("game"))
                {
                    if (auto* pData = pGame->GetBossHudData())
                    {
                        /* Host::Start 済みならその場で再接続 */
                        if (auto* pHost = pGame->GetBossHudScreenHost())
                        {
                            if (auto* pUiScreen = pHost->GetScreen())
                            {
                                auto* pScreen = static_cast<nsApp::nsUI::BossHudScreen*>(pUiScreen);
                                pData->SetScreen(pScreen);
                                pScreen->Bind(pData);
                                pUiScreen->SetVisible(true);
                            }
                        }

                        /* 最後に SetBoss（内部で SyncToScreen） */
                        pData->SetBoss(boss);
                    }
                }
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
