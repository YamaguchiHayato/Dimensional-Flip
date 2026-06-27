#pragma once

#include "IBackGround.h"
#include "Src/Core/CameraManager.h"

namespace nsApp
{
    namespace nsStage
    {
        namespace nsBackGround
        {
            /**
             * @brief 旧通常ステージ背景（Sky.tkm タイル列）。
             *
             * 現在は ScrollStageBackGround に置き換え済み。参照用に残す。
             */
            class NormalBackGround : public IBackGround
            {
            public:
                NormalBackGround() = default;
                virtual ~NormalBackGround() = default;

            public:
                /**
                 * @brief 初期化処理。
                 * @return 成功時 true。
                 */
                bool Start() override;

                /**
                 * @brief 更新処理。
                 */
                void Update() override;

                /**
                 * @brief 描画処理（2D カメラ時のみ）。
                 * @param rc レンダリングコンテキスト。
                 */
                void Render(RenderContext& rc) override;

            public:
                /**
                 * @brief 背景追従の基準 X を上書きする。
                 * @param overridePosition 上書き座標。
                 */
                inline void SetOverrideTrackingPosition(const Vector3& overridePosition)
                {
                    isOverrideActive_ = true;
                    overrideTrackingPosition_ = overridePosition;
                }

                /**
                 * @brief 追従上書きを解除する。
                 */
                inline void ClearOverride() { isOverrideActive_ = false; }

            private:
                CameraManager* pCameraManager_ = nullptr; //!< カメラモード判定用。

            private:
                ModelRender models_[10];           //!< 横タイル列。
                Vector3 overrideTrackingPosition_; //!< 追従上書き座標。
                bool isOverrideActive_ = false;    //!< 追従上書きフラグ。
            };

        } // namespace nsBackGround
    } // namespace nsStage
} // namespace nsApp
