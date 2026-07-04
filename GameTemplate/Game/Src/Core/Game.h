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
#include "Src/UI/HPbarUI.h"
#include "Src/UI/NumberUI.h"
#include "Src/UI/ScoreUI.h"
#include "Src/UI/TimerUI.h"
#include "Src/UI/UIBase.h"
#include "stdint.h"

class CameraManager;
class TrackingEnemy;

namespace nsApp
{
    namespace nsUI
    {
        class BossHudScreenHost;
    }
    namespace nsPresentation
    {
        class BossHudData;
    }

    namespace nsCore
    {
        /**
         * @class Game
         * @brief インゲーム本体。プレイヤー・UI・ステージ遷移を統括する GO。
         */
        class Game : public IGameObject
        {
        public:
            /* コンストラクタとデストラクタ。 */
            Game() = default;
            virtual ~Game();

            /**
             * @brief ゲームを開始する。
             * @return ゲーム開始に成功した場合は true、失敗した場合は false。
             */
            bool Start();

            /**
             * @brief ゲームを更新する。
             */
            void Update();

            /**
             * @brief ゲームを描画する。
             * @param nextStageID 次のステージ ID。
             */
            void RequestStageTransition(nsStage::StageID nextStageID);

            /**
             * @brief ステージの背景をリフレッシュする。
             * @param stageID ステージ ID。
             */
            void RefreshStageBackGround(nsStage::StageID stageID);

            /**
             * @brief ステージ遷移中かどうかを取得する。
             * @return ステージ遷移中の場合は true、そうでない場合は false。
             */
            inline bool IsStageTransitioning() const { return state_ != SceneTransitionState::None; }

            /**
             * @brief 次のステージ ID を取得する。
             * @return 次のステージ ID。遷移中でない場合は sInvalid。
             */
            inline nsApp::nsStage::StageID GetNextStageID() const { return nextStageID_; }

            /**
             * @brief カメラの次元を変更する。
             * @param mode カメラモード。2D か 3D かを指定する。
             */
            void ChangeDimension(CameraMode mode);

            /**
             * @brief ボス HUD 用 ScreenHost を取得する。
             * @return BossHudScreenHost。未生成なら nullptr。
             */
            nsUI::BossHudScreenHost* GetBossHudScreenHost() { return buildHelper_.GetBossHudScreenHost(); }

            /**
             * @brief ボス HUD 用 HP データを取得する。
             * @return BossHudData へのポインタ。
             */
            nsPresentation::BossHudData* GetBossHudData() { return buildHelper_.GetBossHudData(); }


        private:
            /* ステージ遷移の状態を表す列挙型。 */
            void PlayerCreateInstance();
            void UICreateInstance();
            void TimerCreateInstance();
            void NumberCreateInstance();
            void ScoreCreateInstance();
            void HPbarCreateInstance();
            void UpdateTransition();


        private:
            InGameBuildHelper buildHelper_;

            TimerUI* pTimerUI_ = nullptr;
            NumberUI* pNumberUI_ = nullptr;
            ScoreUI* pScoreUI_ = nullptr;
            HPbarUI* pHpbarUI_ = nullptr;
            Fade* pFade_ = nullptr;
            Player* pPlayer_ = nullptr;
            CameraManager* pCameraManager_;
            nsStage::nsBackGround::IBackGround* pBackGrounds_ = nullptr;
            nsK2EngineLow::GameSoundEngine* pSoundEngine_ = nullptr;

            SceneTransitionState state_ = SceneTransitionState::None;
            nsApp::nsStage::StageID nextStageID_ = nsApp::nsStage::StageID::sInvalid;
            Stopwatch stageClearTimer_;

            uint8_t currentStageNum_ = -1;

            bool m_isFadeInEnd = false;
            bool m_hasAppliedStageBgm_ = false;

            enum class StageLoadPhase : uint8_t
            {
                Idle,
                Worker,
                Main,
            };

            StageLoadPhase stageLoadPhase_ = StageLoadPhase::Idle;
            uint64_t stageLoadWorkerJobId_ = 0;
            uint64_t stageLoadMainJobId_ = 0;
            bool isStageLoadMainEnqueued_ = false;
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
