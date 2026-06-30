#pragma once

#include "Src/UI/BossUI/BossUIBase.h"

/**
 * @file   BossPhaseUI.h
 * @brief  ボス戦フェーズ表示・GO 演出 UI。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @enum BossPhaseKind
         * @brief ボス戦フェーズの種類。
         */
        enum class BossPhaseKind : uint8_t
        {
            Phase1 = 0, //!< フェーズ1（2D 視点）
            Phase2,     //!< フェーズ2（3D 視点）
            Phase3,     //!< フェーズ3（ミックス視点）
            GO,         //!< GO 表示用スプライト
            None,
            Num
        };

        /**
         * @enum PhaseAnimState
         * @brief フェーズ開始演出の進行状態。
         */
        enum class PhaseAnimState : uint8_t
        {
            None = 0, //!< 非表示
            Phase,    //!< フェーズ番号を中央に大表示
            Moving,   //!< 右下へ移動・縮小
            GO,       //!< GO 表示中
            Finish,   //!< 演出終了
            Num
        };

        /**
         * @class BossPhaseUI
         * @brief PHASE 表示 → 移動 → GO のシーケンス演出を制御する。
         */
        class BossPhaseUI : public BossUIBase
        {
        public:
            /* コンストラクタとデストラクタ。*/
            BossPhaseUI() = default;
            virtual ~BossPhaseUI() = default;

            /**
             * @brief フェーズ画像・看板スプライトを読み込む。
             */
            void Initialize();

            /**
             * @brief 演出ステートとレイアウトを更新する。
             */
            void Update() override;

            /**
             * @brief フェーズ番号・看板・GO を描画する。
             * @param rc レンダリングコンテキスト。
             */
            void Draw(RenderContext& rc) override;

            /**
             * @brief 現在のフェーズを設定する。
             * @param phase フェーズ種別。
             */
            inline void SetPhase(BossPhaseKind phase) { currentPhase_ = phase; }

            /**
             * @brief GO スプライトの表示を開始する。
             */
            void PlayGOSprite();

            /**
             * @brief GO 表示タイマーを更新する。
             * @param basePos 基準ワールド座標。
             */
            void UpdateGOTimer(Vector3& basePos);

            /**
             * @brief フェーズ切り替え演出を開始する。
             * @param phase 表示するフェーズ。
             */
            void StartPhaseAninm(BossPhaseKind phase);

            /**
             * @brief 演出ステートマシンを1フレーム進める。
             */
            void UpdatePhaseAnim();

            /**
             * @brief フェーズ演出が再生中かどうか。
             * @return Phase / Moving / GO 中なら true。
             */
            inline bool IsPhasePlaying() const
            {
                return phaseState_ != PhaseAnimState::None && phaseState_ != PhaseAnimState::Finish;
            }

            /**
             * @brief フェーズ表示中にアクター停止が必要か。
             * @return Phase ステート中なら true。
             */
            inline bool ShouldStopActors() const { return phaseState_ == PhaseAnimState::Phase; }

        private:
            void UpdatePhaseState(float deltaTime);
            void UpdateMovingState(float deltaTime, const Vector3& basePos);
            void UpdateGOState();
            void UpdateFinishState(const Vector3& basePos);
            void UpdateAfterState(const Vector3& basePos);
            void UpdateLayOut();
            void UpdateLayOutLater();

        private:
            SpriteRender phaseIcons_[(int) BossPhaseKind::Num]; //!< フェーズ数字スプライト。
            SpriteRender phaseIconRender_;                      //!< 「PHASE」看板。

            BossPhaseKind currentPhase_ = BossPhaseKind::Phase1; //!< 現在フェーズ。

            float goTimer_ = 0.0f;                 //!< GO 表示残り時間。
            bool isGOPlaying_ = false;             //!< GO 再生中フラグ。
            Vector3 basePosition_ = Vector3::Zero; //!< 基準位置（予約）。

            PhaseAnimState phaseState_ = PhaseAnimState::None; //!< 演出ステート。
            float animTimer_ = 0.0f;                           //!< ステート内タイマー。
            Vector3 currentPosition_ = Vector3::Zero;          //!< 看板ローカル位置。
            Vector3 currentScale_ = Vector3::Zero;             //!< 看板スケール。
            Vector3 currentNumberOffset_ = Vector3::Zero;      //!< 数字のオフセット。
        };
    } // namespace nsUI
} // namespace nsApp

using BossPhaseKind = nsApp::nsUI::BossPhaseKind;
using PhaseAnimState = nsApp::nsUI::PhaseAnimState;
using BossPhaseUI = nsApp::nsUI::BossPhaseUI;

namespace app
{
    namespace nsUI
    {
        using BossPhaseKind = nsApp::nsUI::BossPhaseKind;
        using PhaseAnimState = nsApp::nsUI::PhaseAnimState;
        using BossPhaseUI = nsApp::nsUI::BossPhaseUI;
    } // namespace nsUI
} // namespace app
