#pragma once

#include <vector>

#include "IBackGround.h"
#include "Src/Actor/Stage/BackGround/ScrollLayer.h"

namespace nsApp
{
    namespace nsStage
    {
        namespace nsScrollBackGround
        {
            /**
             * @brief 通常ステージ用のスクロール背景（Sky / Mountain / Ground）。
             *
             * IBackGround として Game から生成され、2D / 3D 共通で動作する。
             * 実描画は RenderToMainTarget() で Forward 直前に行う。
             */
            class ScrollStageBackGround : public nsBackGround::IBackGround
            {
            public:
                ScrollStageBackGround() = default;
                virtual ~ScrollStageBackGround() = default;

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
                 * @brief 描画処理（IGameObject 用。即時描画は行わない）。
                 * @param rc レンダリングコンテキスト。
                 */
                void Render(RenderContext& rc) override;

                /**
                 * @brief メインレンダーターゲットへ背景を描画する。
                 * @param rc レンダリングコンテキスト。
                 * @param mainRT メインレンダリングターゲット。
                 */
                void RenderToMainTarget(RenderContext& rc, RenderTarget& mainRT);

                /**
                 * @brief 背景追従の基準 X を上書きする（スター取得演出など）。
                 * @param overridePosition 上書き座標（X のみ使用）。
                 */
                void SetOverrideTrackingPosition(const Vector3& overridePosition);

                /**
                 * @brief 追従上書きを解除する。
                 */
                void ClearOverride();


            private:
                /**
                 * @brief 更新・描画に使うカメラ基準 X を取得する。
                 * @return カメラ基準のワールド X。
                 */
                float GetCameraWorldX() const;

                /**
                 * @brief 可視範囲の半幅を取得する。
                 * @return ワールド単位の半幅。
                 */
                float GetViewHalfWidth() const;

            private:
                std::vector<ScrollLayer> layers_;                  //!< 背景レイヤー列。
                Vector3 overrideTrackingPosition_ = Vector3::Zero; //!< 追従上書き座標。
                bool isOverrideActive_ = false;                    //!< 追従上書きフラグ。
            };

        } // namespace nsScrollBackGround
    } // namespace nsStage
} // namespace nsApp
