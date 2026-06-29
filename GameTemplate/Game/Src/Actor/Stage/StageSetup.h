#pragma once

#include "Src/Actor/Stage/StageID.h"

namespace nsApp
{
    namespace nsStage
    {
        /**
         * @class StageSetup
         * @brief ステージ入場時のルール（ボス戦カメラ・カットイン・Pause など）を担当する。
         * @note  配置物は StageSpawn.tsv + StageObjectSpawner が担当。ここは「ルール」専用。
         */
        class StageSetup
        {
        public:
            /** @brief ステージに入った直後に呼ぶ。 */
            void OnEnter(StageID stageId);

            /** @brief ステージを出る直前に呼ぶ。 */
            void OnLeave(StageID stageId);

            /** @brief 毎フレーム更新（カットイン終了判定・BossUI 更新）。 */
            void Update();

            /** @brief 毎フレーム描画（Boss UI）。 */
            void Render(RenderContext& rc);

            /**
             * @brief Game.cpp の FadeIn で Player の Pause を解除していいか。
             * @return ボス戦カットイン中は true（Pause を維持する）。
             */
            static bool ShouldKeepPlayerPaused();

        private:
            bool isBossStage_ = false;    //! いまボスステージか。
            bool isCutInPlaying_ = false; //! カットイン再生中か。
        };
    } // namespace nsStage
} // namespace nsApp
