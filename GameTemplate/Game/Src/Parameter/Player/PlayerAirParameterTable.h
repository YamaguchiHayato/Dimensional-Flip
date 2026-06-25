#pragma once

#include "Src/Parameter/ParameterSystemTableBase.h"

namespace nsApp
{
    namespace nsSystem
    {
        /**
         * @struct PlayerAirParameter 
         * @brief Player の空中挙動パラメータ構造体。
         */
        struct PlayerAirParameter
        {
            float jumpPower = 150.0f;        //! ジャンプ力。
            float bounceJumpPower = 100.0f;  //! バウンドジャンプ力。
            float gravityBase = 8.0f;        //! 重力加速度。
            float jumpGravityScale = 1.2f;   //! ジャンプ中の重力倍率。
            float jumpCutScale = 2.0f;       //! ジャンプカット時の重力倍率。
            float maxFallSpeed = -200.0f;    //! 最大落下速度。
            float airMoveRateJump = 0.7f;    //! 空中移動率（ジャンプ中）。
            float airMoveRateFall = 0.4f;    //! 空中移動率（落下中）。
            float bounceCooldown = 0.2f;     //! バウンドジャンプのクールタイム。
        };


        class PlayerAirParameterTable : private ParameterTableBase<std::string, PlayerAirParameter>
        {
            using Base = ParameterTableBase<std::string, PlayerAirParameter>;

        public:
            /**
             * @brief TSV ファイルを読み込む。
             * @param filePath TSV ファイルのパス。
             * @return 読み込みに成功した場合は true、失敗した場合は false
             */
            static bool LoadTSVFile(const char* filePath);

            /**
             * @brief PlayerAirParameter の参照を取得する。
             * @return PlayerAirParameter への参照
             */
            static const PlayerAirParameter& Get();
        };
    } // namespace nsSystem
} // namespace nsApp
