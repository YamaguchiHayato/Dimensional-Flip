#include "stdafx.h"

#include "Src/Actor/Stage/StageID.h"
#include "StageIcon.h"

namespace nsApp
{
    namespace nsUI
    {
        void StageIcon::Init(const char* modelPath, StageID stageID)
        {
            /**
             * @brief プレビュー用モデルを読み込み縮小表示する
             */
            model_.Init(modelPath);
            model_.SetScale(Vector3::One * 0.25f);
            stageID_ = stageID;
        }


        void StageIcon::Update()
        {
            /**
             * @brief シーンから渡された座標にモデルを配置する
             */
            model_.SetPosition(position_);
            model_.SetRotation(Quaternion::Identity);
            model_.Update();
        }


        void StageIcon::Render(RenderContext& rc)
        {
            model_.Draw(rc);
        }
    } // namespace nsUI
} // namespace nsApp
