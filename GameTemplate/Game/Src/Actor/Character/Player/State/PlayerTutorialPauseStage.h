#pragma once

#include "Src/Actor/Character/IState.h"

/**
 * @file   PlayerTutorialPauseStage.h
 * @brief  チュートリアル専用のプレイヤーステート。
 */

namespace nsApp
{
    namespace nsActor
    {
        namespace nsCharacter
        {
            namespace nsPlayer
            {
                class Player;
            } // namespace nsPlayer
        } // namespace nsCharacter
    } // namespace nsActor

    namespace nsUI
    {
        class GameplayHudScreenHost;
        class TutorialMessageUI;
        class TutorialSequencer;
        class TutorialCompleteUI;
    } // namespace nsUI

    namespace nsState
    {
        /**
         * @class PlayerTutorialPauseStage
         * @brief チュートリアル中の移動・ジャンプ・視点切替を行い、UI シーケンサーと連携する。
         */
        class PlayerTutorialPauseStage : public IState
        {
        public:
            /**
             * @brief コンストラクタ。
             * @param pPlayer 所属するプレイヤーへのポインタ。
             *
             * @note PlayerForward.h の typedef ではなく完全修飾名を使う（C2665 対策）。
             *       実装は .cpp 側に置く（Emplace / make_unique 用）。
             */
            explicit PlayerTutorialPauseStage(nsApp::nsActor::nsCharacter::nsPlayer::Player* pPlayer);

            /**
             * @brief デストラクタ。
             */
            ~PlayerTutorialPauseStage() override;


        public:
            /**
             * @brief ステートに入るときの処理。
             */
            void Enter() override;

            /**
             * @brief ステート中の更新処理。
             */
            void Update() override;

            /**
             * @brief ステートから抜けるときの処理。
             */
            void Exit() override;

            /**
             * @brief ステートの ID を要求する。
             * @param request 要求するステート ID を格納する変数への参照。
             * @return 要求が成功した場合は true、失敗した場合は false。
             */
            bool RequestID(uint8_t& request) override;

        private:
            /**
             * @brief チュートリアル中のアニメーション更新処理。
             */
            void AnimationUpdatingProcess();

            /**
             * @brief チュートリアル中の移動更新処理。
             */
            void JumpUpdatingProcessAtTutorial();

            /**
             * @brief チュートリアル中の視点切替更新処理。
             */
            void ViewpointChangeUpdatingProcessAtTutorial();

            /**
             * @brief チュートリアル中の UI シーケンサー更新処理。
             */
            void TutorialTextChange();


        private:
            nsApp::nsActor::nsCharacter::nsPlayer::Player* pPlayer_ = nullptr; //! < 所属するプレイヤー。
            nsUI::TutorialMessageUI* pTutorialMessageUI_ = nullptr;            //! < チュートリアルメッセージ UI。
            nsUI::TutorialSequencer* pTutorialSequencer_ = nullptr;            //! < チュートリアルシーケンサー。
            nsUI::TutorialCompleteUI* pTutorialCompleteUI_ = nullptr;          //! < チュートリアル完了 UI。
            nsUI::GameplayHudScreenHost* pGameplayHudHost_ = nullptr;          //! < ゲームプレイ HUD スクリーンホスト。

        private:
            bool isButtonPressed_ = false;          //! < ボタンが押されたかどうか。
            bool isVisible_ = false;                //! < チュートリアルメッセージが表示されているかどうか。
            bool isGround_ = false;                 //! < プレイヤーが地面にいるかどうか。
            float currentTime_ = 0.0f;              //! < 現在の時間。
            float tutorialTimer_ = 0.0f;            //! < チュートリアルのタイマー。
            float speedRate_ = 1.0f;                //! < 移動速度の倍率。
            Vector3 tutorialStick_ = Vector3::Zero; //! < チュートリアル中のスティック入力。
        };
    } // namespace nsState
} // namespace nsApp
