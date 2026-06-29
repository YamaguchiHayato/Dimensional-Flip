#pragma once

#include <string>

#include "Src/Actor/Stage/StageID.h"

namespace nsApp
{
    namespace nsSystem
    {
        /**
         * @enum StageID 
         * @brief ステージマスターレコード構造体。
         * @details ステージのID、モデルパス、プレイヤーの初期位置、スケール、位置、Y軸回転角度、背景タイプを保持する。
         */
        struct StageMasterRecord
        {
            nsStage::StageID stageID = nsStage::StageID::sInvalid;                        //! ステージのID。
            std::string modelPath;                                      //! モデルのパス。
            Vector3 playerStartPosition = Vector3(0.0f, 20.0f, 0.0f);   //! プレイヤーの初期位置。
            Vector3 scale = Vector3::One;                               //! ステージのスケール。
            Vector3 position = Vector3::Zero;                           //! ステージの位置。
            float rotY = 0.0f;                                          //! Y軸回転角度。
            std::string backgroundType;                                 //! 背景のタイプ。
        };
    } // namespace nsSystem
} // namespace nsApp
