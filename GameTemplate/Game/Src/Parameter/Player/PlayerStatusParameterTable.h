#pragma once

#include "Src/Parameter/ParameterSystemTableBase.h"

namespace nsApp
{
    namespace nsSystem
    {
        /**
         * @struct PlayerStatusParameter 
         * @brief プレイヤーのステータスパラメータを格納する構造体。
         * @note
         * この構造体はプレイヤーの最大HP、攻撃力、無敵時間、落下時のリスポーンダメージなどのパラメータを保持する。
         * @date 2026/06/25
         */
        struct PlayerStatusParameter
        {
            int maxHP = 9;               //! プレイヤーの最大HP。
            float attackPower = 1.0f;    //! プレイヤーの攻撃力。
            float invincibleTime = 1.0f; //! プレイヤーがダメージを受けた後の無敵時間（秒）。
            int fallRespawnDamage = 1;   //! プレイヤーが落下した際のリスポーン時のダメージ。
        };


        class PlayerStatusParameterTable : private ParameterTableBase<std::string, PlayerStatusParameter>
        {
            using Base = ParameterTableBase<std::string, PlayerStatusParameter>;

        public:
            /**
             * @brief TSV ファイルからプレイヤーのステータスパラメータを読み込む。
             * @param filePath TSV ファイルのパス。
             * @return 読み込みが成功した場合は true、失敗した場合は false を返す。
             */
            static bool LoadTSVFile(const char* filePath);

            /**
             * @brief プレイヤーのステータスパラメータを取得する。
             * @return プレイヤーのステータスパラメータへの参照。
             */
            static const PlayerStatusParameter& Get();
        };
    } // namespace nsSystem
} // namespace nsApp
