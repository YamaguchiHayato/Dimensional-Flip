#pragma once

#include <string>

namespace nsApp
{
    namespace nsSystem
    {
        /**
         * @struct StageSpawnRecord
         * @brief StageSpawn.tsv の1行分（配置オブジェクト定義）。
         * @note  ObjectType に応じて StageObjectSpawner が生成する。
         * @date  2026/06/29
         */
        struct StageSpawnRecord
        {
            std::string stageId;    //! ステージ ID 文字列（例: Stage1）。
            std::string objectType; //! 生成種別（例: Star, JumpPad）。
            float posX = 0.0f;      //! 配置 X。
            float posY = 0.0f;      //! 配置 Y。
            float posZ = 0.0f;      //! 配置 Z。
            float param0 = 0.0f;    //! 拡張パラメータ 0。
            float param1 = 0.0f;    //! 拡張パラメータ 1。
            float param2 = 0.0f;    //! 拡張パラメータ 2。
            std::string groupId;    //! ギミック連携用グループ ID。
        };
    } // namespace nsSystem
} // namespace nsApp
