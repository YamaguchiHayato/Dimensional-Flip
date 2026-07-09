#pragma once

#include "Src/Actor/Stage/StageID.h"
#include "Src/Presentation/Data/BossBriefingData.h"
#include "Src/UI/BossTutorial/BossBriefingController.h"

namespace nsApp
{
    namespace nsStage
    {
        /**
         * @enum BossIntroPhase
         * @brief ボスステージ入場後の進行フェーズ。
         */
        enum class BossIntroPhase : uint8_t
        {
            CutIn,    //!< カットイン再生中。
            Briefing, //!< スライド説明中。
            Battle,   //!< 本番ボス戦。
        };

        /**
         * @class StageSetup
         * @brief ステージ入場時のルール（ボス戦カメラ・カットイン・Briefing など）。
         */
        class StageSetup
        {
        public:
            /**
             * @brief ステージ入場時の初期化処理。
             * @param stageId 入場するステージ ID。
             */
            void OnEnter(StageID stageId);

            /**
             * @brief ステージ離脱時の終了処理。
             * @param stageId 離脱するステージ ID。
             */
            void OnLeave(StageID stageId);

            /**
             * @brief ステージ入場後の更新処理。ボス戦カットイン・Briefing などを管理する。
             */
            void Update();

            /**
             * @brief ステージ入場後の描画処理。ボス戦カットイン・Briefing などを管理する。
             * @param rc RenderContext 描画コンテキスト。
             */
            void Render(RenderContext& rc);

            /**
             * @brief ボス戦カットイン・Briefing 中はプレイヤーを止めるかどうか。    
             * @return true: プレイヤーを止める, false: プレイヤーを止めない
             */
            static bool ShouldKeepPlayerPaused();


        private:
            /**
             * @brief ボス戦スライド説明用の画像パスを初期化する。
             */
            void InitBossBriefingSlides();

            /**
             * @brief ボス戦スライド説明用の Host を生成する。
             */
            void StartBriefing();

            /**
             * @brief ボス戦本番を開始する。
             */
            void StartBattle();


        private:
            bool isBossStage_ = false; //! <! ボスステージかどうか。
            bool s_isBriefing = false; //! < ボス戦スライド説明中かどうか。
            BossIntroPhase introPhase_ = BossIntroPhase::CutIn; //! < ボス戦入場後の進行フェーズ。

            nsPresentation::BossBriefingData briefingData_; //! < Briefing 用のスライドデータ。
            nsCore::BossBriefingController briefingController_; //! < Briefing 用のコントローラー。
        };
    } // namespace nsStage
} // namespace nsApp
