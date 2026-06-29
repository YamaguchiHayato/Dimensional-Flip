#pragma once

#include <cstdint>

namespace nsApp
{
    namespace nsStage
    {
        /**
         * @enum StageID
         * @brief ステージを識別する列挙型（旧 IStage.h から移動）。
         */
        enum class StageID : uint8_t
        {
            sTutorialStage, //! チュートリアル。
            sStage1,        //! ステージ1。
            sStage2,        //! ステージ2。
            sStage3,        //! ステージ3。
            sStageEX,       //! ボスステージ。
            sInvalid        //! 無効。
        };
    }
}

using StageID = nsApp::nsStage::StageID;
