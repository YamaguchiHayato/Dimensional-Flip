#pragma once
#include "stdint.h"
#include "GameSoundEngine.h"
#include "Src/Actor/Stage/StageID.h"
#include <DirectXTK/Inc/Audio.h>
#include <memory>

// ゲーム管理クラス。
#include "Src/Core/CameraManager.h"
#include "Src/Core/SceneManager.h"


// UIクラス。
#include "Src/UI/HPbarUI.h"
#include "Src/UI/NumberUI.h"
#include "Src/UI/ScoreUI.h"
#include "Src/UI/TimerUI.h"

#include "IBackGround.h"
#include "Src/Actor/Character/PlayerForward.h"
#include "Src/Build/InGameBuildHelper.h"
#include "Src/Core/Job/JobHandle.h"

class CameraManager;
class TrackingEnemy;
class Fade;

// UI。
class NumberUI;
class ScoreUI;
class TimerUI;
class HPbarUI;

namespace nsApp {
    class InGameBuildHelper;
}

namespace app
{
    namespace core
    {
        // データ統合クラス。
        class Game : public IGameObject
        {
        public:
            Game() = default;
            virtual ~Game();


        public:
            /**
             * @brief ゲームの初期化処理。
             * @return 初期化に成功した場合は true、失敗した場合は false。
             */
            bool Start();

            /**
             * @brief ゲームの更新処理。
             */
            void Update();


        public:
            /**
             * @brief ステージ遷移をリクエストする。
             * @param nextStageID 次のステージID。
             */
            void RequestStageTransition(nsApp::nsStage::StageID nextStageID);


        public:
            /**
             * @brief ステージ遷移中かどうかを判定する。
             * @return ステージ遷移中であれば true、そうでなければ false。
             */
            inline bool IsStageTransitioning() const
            {
                return state_ != SceneTransitionState::None;
            }

            /**
             * @brief 次のステージIDを取得する。
             * @return 次のステージID。
             */
            inline nsApp::nsStage::StageID GetNextStageID() const
            {
                return nextStageID_;
            }

            /**
             * @brief カメラの次元を変更する。
             * @param mode 変更するカメラモード。
             */
            void ChangeDimension(CameraMode mode);


        private:
            // Player。
            void PlayerCreateInstance();
            // UI。
            inline void UICreateInstance();
            // 各UIの生成。
            // タイマーUI。
            inline void TimerCreateInstance();
            // ナンバーUI。
            inline void NumberCreateInstance();
            // スコアUI。
            inline void ScoreCreateInstance();
            // HPbar。
            inline void HPbarCreateInstance();


        private:
            // 遷移時の更新処理。
            void UpdateTransition();

            

        private:
            nsApp::InGameBuildHelper buildHelper_;

            TimerUI* pTimerUI_ = nullptr;
            NumberUI* pNumberUI_ = nullptr;
            ScoreUI* pScoreUI_ = nullptr;
            HPbarUI* pHpbarUI_ = nullptr;
            Fade* pFade_ = nullptr;
            Player* pPlayer_ = nullptr;
            CameraManager* pCameraManager_;
            nsApp::nsStage::nsBackGround::IBackGround* pBackGrounds_ = nullptr;
            nsK2EngineLow::GameSoundEngine* pSoundEngine_ = nullptr;


       // 列挙型クラス。
        private:
            // 遷移管理用メンバ。
            SceneTransitionState state_ = SceneTransitionState::None;
            nsApp::nsStage::StageID nextStageID_ = nsApp::nsStage::StageID::sInvalid;
            Stopwatch stageClearTimer_;


        private:
            // 現在のステージ番号を追跡するための変数。
            uint8_t currentStageNum_ = -1;


        private:

            // フェードが終了しているかどうか
            bool m_isFadeInEnd = false;
            bool m_hasAppliedStageBgm_ = false;


        private:
            /**
             * @brief ステージロード Job の内部フェーズ。
             */
            enum class StageLoadPhase : uint8_t
            {
                Idle,   //! Worker Job をまだ投げていない。
                Worker, //! Worker 完了待ち。
                Main,   //! Main Job 完了待ち。
            };

            StageLoadPhase stageLoadPhase_ = StageLoadPhase::Idle;
            uint64_t stageLoadWorkerJobId_ = 0;
            uint64_t stageLoadMainJobId_ = 0;
            bool isStageLoadMainEnqueued_ = false;
        };
    }
}

