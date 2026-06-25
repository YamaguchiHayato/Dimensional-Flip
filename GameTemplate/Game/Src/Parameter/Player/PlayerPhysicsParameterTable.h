#pragma once

#include "Src/Parameter/ParameterSystemTableBase.h"

namespace nsApp
{
    namespace nsSystem
    {
        /**
         * @struct PlayerPhysicsParameter 
         * @brief Player の物理パラメータを格納する構造体。
         * @note この構造体は PlayerPhysicsParameterTable で管理される。
         */
        struct PlayerPhysicsParameter
        {
            float colliderRadius = 2.0f;          //! Player のコライダー半径。
            float colliderHeight = 1.0f;          //! Player のコライダー高さ。
            float fixedDeltaTime = 1.0f / 150.0f; //! Player の固定フレーム時間。
            float deadLineY = -100.0f;            //! Player が死亡する Y 座標の閾値。
        };

        class PlayerPhysicsParameterTable : private ParameterTableBase<std::string, PlayerPhysicsParameter>
        {
            using Base = ParameterTableBase<std::string, PlayerPhysicsParameter>;

        public:
            /**
             * @brief TSV ファイルから PlayerPhysicsParameter を読み込む。
             * @param filePath TSV ファイルのパス。
             * @return 読み込みに成功した場合は true、失敗した場合は false。
             */
            static bool LoadTSVFile(const char* filePath);

            /**
             * @brief PlayerPhysicsParameter を取得する。
             * @return PlayerPhysicsParameter への参照。
             */
            static const PlayerPhysicsParameter& Get();
        };
    } // namespace nsSystem
} // namespace nsApp
