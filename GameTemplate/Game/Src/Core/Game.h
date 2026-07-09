#pragma once

#include <DirectXTK/Inc/Audio.h>
#include <memory>

#include "GameSoundEngine.h"
#include "IBackGround.h"
#include "Src/Actor/Character/PlayerForward.h"
#include "Src/Actor/Stage/StageID.h"
#include "Src/Build/InGameBuildHelper.h"
#include "Src/Core/CameraManager.h"
#include "Src/Core/Job/JobHandle.h"
#include "Src/Core/SceneManager.h"
#include "Src/Production/Fade.h"
#include "Src/UI/Pause/PauseController.h"
#include "Src/UI/Pause/SoundSettingController.h"
#include "Src/UI/UIBase.h"
#include "stdint.h"

class CameraManager;
class TrackingEnemy;

namespace nsApp
{
    namespace nsUI
    {
        class BossHudScreenHost;
        class GameplayHudScreenHost;
        class SoundSettingScreenHost;
    } // namespace nsUI
    namespace nsPresentation
    {
        class BossHudData;
        class SoundSettingData;
    } // namespace nsPresentation

    namespace nsCore
    {
        /**
         * @class Game
         * @brief インゲーム本体。プレイヤー・UI・ステージ遷移を統括する GO。
         */
        class Game : public IGameObject
        {
        public:
            /** @brief コンストラクタ。 */
            Game() = default;

            /** @brief デストラクタ。生成した HUD / GO を破棄する。 */
            virtual ~Game();

        public:
            /**
             * @brief ゲームを開始する。
             * @return 成功した場合 true、失敗した場合 false。
             */
            bool Start();

            /**
             * @brief ゲームを更新する。
             */
            void Update();

        public:
            /**
             * @brief ステージ遷移を要求する。
             * @param[in] nextStageID 次のステージ ID。
             */
            void RequestStageTransition(nsStage::StageID nextStageID);

            /**
             * @brief ステージ背景を作り直す。
             * @param[in] stageID ステージ ID。
             */
            void RefreshStageBackGround(nsStage::StageID stageID);

            /**
             * @brief ステージ遷移中かどうかを返す。
             * @return 遷移中なら true。
             */
            inline bool IsStageTransitioning() const { return state_ != SceneTransitionState::None; }

            /**
             * @brief 次のステージ ID を返す。
             * @return 遷移中でない場合は sInvalid。
             */
            inline nsApp::nsStage::StageID GetNextStageID() const { return nextStageID_; }

            /**
             * @brief カメラの次元を切り替える。
             * @param[in] mode 2D / 3D モード。
             */
            void ChangeDimension(CameraMode mode);

            /**
             * @brief ボス HUD 用 ScreenHost を返す。
             * @return 未生成なら nullptr。
             */
            nsUI::BossHudScreenHost* GetBossHudScreenHost() { return buildHelper_.GetBossHudScreenHost(); }

            /**
             * @brief ボス HUD 用データを返す。
             * @return BossHudData へのポインタ。
             */
            nsPresentation::BossHudData* GetBossHudData() { return buildHelper_.GetBossHudData(); }

            /**
             * @brief ゲームプレイ HUD 用 ScreenHost を返す。
             * @return 未生成なら nullptr。
             */
            nsUI::GameplayHudScreenHost* GetGameplayHudScreenHost() { return buildHelper_.GetGameplayHudScreenHost(); }

            /**
             * @brief ゲームプレイ HUD 用データを返す。
             * @return GameplayHudData へのポインタ。
             */
            nsPresentation::GameplayHudData* GetGameplayHudData() { return buildHelper_.GetGameplayHudData(); }

            /**
             * @brief Sound Setting 用 ScreenHost を返す。
             * @return 未生成なら nullptr。
             */
            nsUI::SoundSettingScreenHost* GetSoundSettingScreenHost()
            {
                return buildHelper_.GetSoundSettingScreenHost();
            }

            /**
             * @brief Sound Setting 用データを返す。
             * @return SoundSettingData へのポインタ。
             */
            nsPresentation::SoundSettingData* GetSoundSettingData() { return buildHelper_.GetSoundSettingData(); }

            /**
             * @brief ステージタイマーをリセットする。
             */
            void ResetStageTimer();

            /**
             * @brief ステージタイマーを更新する。
             */
            void UpdateStageTimer();

            /**
             * @brief 現在ステージをフェードなしで即時リスタートする。
             */
            void RestartCurrentStageImmediate();

        private:
            /**
             * @brief プレイヤー GO を生成する。
             */
            void PlayerCreateInstance();

            /**
             * @brief ステージ遷移ステートを更新する。
             */
            void UpdateTransition();

        private:
            InGameBuildHelper buildHelper_; //!< InGame 起動時の段階的初期化。

            Fade* pFade_ = nullptr;                                                   //!< フェード演出。
            Player* pPlayer_ = nullptr;                                               //!< プレイヤー。
            CameraManager* pCameraManager_;                                           //!< カメラマネージャ。
            nsStage::nsBackGround::IBackGround* pBackGrounds_ = nullptr;              //!< ステージ背景。
            nsK2EngineLow::GameSoundEngine* pSoundEngine_ = nullptr;                  //!< サウンドエンジン（旧互換）。
            nsCore::PauseController pauseController_;                                 //!< ポーズ入力・開閉。
            nsCore::SoundSettingController soundSettingController_;                   //!< Setting 画面の開閉。
            SceneTransitionState state_ = SceneTransitionState::None;                 //!< ステージ遷移ステート。
            nsApp::nsStage::StageID nextStageID_ = nsApp::nsStage::StageID::sInvalid; //!< 遷移先ステージ。
            nsScene::SceneID pendingPauseScene_ = nsScene::SceneID::sInvalid;         //!< ポーズ由来のシーン遷移。

            Stopwatch stageClearTimer_; //!< ステージクリア演出用。

            uint8_t currentStageNum_ = -1; //!< 現在ステージ番号（旧互換）。

            bool m_isFadeInEnd = false;         //!< 初回フェードイン開始済みか。
            bool m_hasAppliedStageBgm_ = false; //!< 現在ステージ BGM 適用済みか。
            float stageTimer_ = 90.0f;          //!< ステージ制限時間。
            bool timeUpFlag_ = false;           //!< タイムアップ済みか。

            /**
             * @enum StageLoadPhase
             * @brief 非同期ステージロードの内部フェーズ。
             */
            enum class StageLoadPhase : uint8_t
            {
                Idle,   //!< 待機。
                Worker, //!< Worker Job 実行中。
                Main,   //!< Main Job 実行中。
            };

            StageLoadPhase stageLoadPhase_ = StageLoadPhase::Idle; //!< 現在のロードフェーズ。
            uint64_t stageLoadWorkerJobId_ = 0;                    //!< Worker Job ID。
            uint64_t stageLoadMainJobId_ = 0;                      //!< Main Job ID。
            bool isStageLoadMainEnqueued_ = false;                 //!< Main Job 投入済みか。
        };
    } // namespace nsCore
} // namespace nsApp

using Game = nsApp::nsCore::Game;

namespace app
{
    namespace core
    {
        using Game = nsApp::nsCore::Game;
    }
} // namespace app
