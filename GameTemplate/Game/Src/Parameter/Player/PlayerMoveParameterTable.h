#pragma once

#include "Src/Parameter/ParameterSystemTableBase.h"

namespace nsApp
{
    namespace nsSystem
    {
        /**
         * @brief Player の移動パラメータを格納する構造体。
         */
        struct PlayerMoveParameter
        {
            float walkSpeed = 100.0f; //! 地上歩行速度。
        };

        /**
         * @brief PlayerMoveParameter を格納するパラメータテーブル。
         * @note このクラスは ParameterSystem によって管理される。
         * @date 2026/06/25: 新規作成
         */
        class PlayerMoveParameterTable : private ParameterTableBase<std::string, PlayerMoveParameter>
        {
            using Base = ParameterTableBase<std::string, PlayerMoveParameter>;

        public:
            /**
             * @brief TSV ファイルから PlayerMoveParameter を読み込む。
             * @param filePath TSV ファイルのパス。
             * @return 読み込みに成功した場合は true、失敗した場合は false。
             */
            static bool LoadTSVFile(const char* filePath);

            /**
             * @brief PlayerMoveParameter のインスタンスを取得する。
             * @return PlayerMoveParameter のインスタンスへの参照。
             */
            static const PlayerMoveParameter& Get();
        };
    } // namespace nsSystem
} // namespace nsApp
