#pragma once

#include <string>
#include <vector>

#include "IBackGround.h"
#include "Src/Actor/Stage/BackGround/ScrollLayer.h"
#include "Src/Actor/Stage/StageID.h"

namespace nsApp
{
    namespace nsStage
    {
        namespace nsScrollBackGround
        {
            class ScrollStageBackGround : public nsBackGround::IBackGround
            {
            public:
                /* コンストラクタとデストラクタ。*/
                ScrollStageBackGround() = default;
                virtual ~ScrollStageBackGround() = default;

            public:
                /**
                 * @brief 次のステージIDを設定する。
                 * @param stageID 設定するステージID。
                 */
                static void SetPendingStageID(StageID stageID);

                /**
                 * @brief ステージIDを設定する。
                 * @param stageID 設定するステージID。
                 */
                void SetStageID(StageID stageID);

                /**
                 * @brief 背景の初期化処理を行う。
                 * @return 初期化に成功した場合は true、失敗した場合は false。
                 */
                bool Start() override;

                /**
                 * @brief 背景の更新処理を行う。
                 */
                void Update() override;

                /**
                 * @brief 背景の描画処理を行う。
                 * @param rc 描画コンテキスト。
                 */
                void Render(RenderContext& rc) override;

                /**
                 * @brief メインレンダーターゲットに背景を描画する。
                 * @param rc 描画コンテキスト。
                 * @param mainRT メインレンダーターゲット。
                 */
                void RenderToMainTarget(RenderContext& rc, RenderTarget& mainRT);

                /**
                 * @brief 背景のスクロールをオーバーライドする位置を設定する。
                 * @param overridePosition オーバーライドする位置。
                 */
                void SetOverrideTrackingPosition(const Vector3& overridePosition);

                /**
                 * @brief オーバーライドを解除する。
                 */
                void ClearOverride();


            private:
                /**
                 * @brief スクロールレイヤーを構築する。
                 * @return 構築に成功した場合は true、失敗した場合は false。
                 */
                bool BuildLayers();

                /**
                 * @brief スクロールレイヤーを更新する。
                 * @return 更新に成功した場合は true、失敗した場合は false。
                 */
                float GetCameraWorldX() const;

                /**
                 * @brief スクロールレイヤーの描画順を更新する。
                 * @return 描画順の更新に成功した場合は true、失敗した場合は false。
                 */
                float GetViewHalfWidth() const;


            private:
                StageID stageId_ = StageID::sInvalid;
                std::vector<ScrollLayer> layers_;
                std::vector<std::string> texturePathStorage_;
                std::vector<ScrollLayerDefinition> definitionStorage_;
                Vector3 overrideTrackingPosition_ = Vector3::Zero;
                bool isOverrideActive_ = false;

                static StageID pendingStageID_;
            };

        } // namespace nsScrollBackGround
    } // namespace nsStage
} // namespace nsApp
