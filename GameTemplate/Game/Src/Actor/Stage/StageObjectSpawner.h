#pragma once

#include <vector>

#include "Src/Actor/Stage/StageID.h"

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
             * @param stageId 生成するステージID。
             */
            void Spawn(StageID stageId);

            /**
             * @brief 生成済みのオブジェクトを削除する。
             */
            void Clear();


        private:
            /**
             * @brief StageSpawnRecord に基づき、1つのオブジェクトを生成する。
             * @param rec 生成するオブジェクトの情報を持つ StageSpawnRecord。
             */
            void SpawnOne(const nsSystem::StageSpawnRecord& rec);
            std::vector<IGameObject*> spawnedObjects_;
        };
    } // namespace nsStage
} // namespace nsApp
