#pragma once

/**
 * @file   StageObjectSpawner.h
 * @brief  StageSpawn.tsv に基づくステージオブジェクト生成。
 */

#include <algorithm>
#include <vector>

#include "Src/Actor/Stage/StageID.h"
#include "gameObject/IGameObject.h"

namespace nsApp
{
    namespace nsSystem
    {
        struct StageSpawnRecord;
    }

    namespace nsStage
    {
        /**
         * @class StageObjectSpawner
         * @brief StageSpawn.tsv の1行を ObjectType に応じて NewGO する。
         */
        class StageObjectSpawner
        {
        public:
            /**
             * @brief StageID に応じたオブジェクトを生成する。
             * @param stageId 生成するステージ ID。
             */
            void Spawn(StageID stageId);

            /**
             * @brief 生成済みオブジェクトを DeleteGO し、リストを空にする。
             * @note  自己削除済み GO は Unregister 済みであること。
             */
            void Clear();

            /**
             * @brief 生成済みリストからポインタを除去する（DeleteGO は呼ばない）。
             * @param obj リストから外すオブジェクト。
             */
            void Unregister(IGameObject* obj);

        private:
            /**
             * @brief StageSpawnRecord に基づき、1 オブジェクトを生成する。
             * @param rec TSV 1 行分のレコード。
             */
            void SpawnOne(const nsSystem::StageSpawnRecord& rec);


            std::vector<IGameObject*> spawnedObjects_; //!< Spawn で登録した GO 一覧
        };
    } // namespace nsStage
} // namespace nsApp
