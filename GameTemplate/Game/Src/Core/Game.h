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
#include "stdint.h"

class CameraManager;
class TrackingEnemy;

class NumberUI;
class ScoreUI;
class TimerUI;
class HPbarUI;

namespace nsApp
{
    class InGameBuildHelper;
}

namespace nsApp
{
    namespace nsCore
    {
        /**
         * @class Game
         * @brief インゲーム本体。プレイヤー・UI・ステージ遷移を統括する GO。
         */
        class Game : public IGameObject
        {
        public:
            /* コンストラクタとデストラクタ。*/
            Game() = default;
            virtual ~Game();


        public:
            /**
             * @brief デストラクタ。各種 GO を削除する。
             */
            bool Start();

            /**
             * @brief 初期化処理。プレイヤー・UI・ステージ遷移の初期化を行う。
             */
            void Update();

            /**
             * @brief 毎フレームの更新。プレイヤー・UI・ステージ遷移の更新を行う。
             * @param nextStageID 次ステージ ID。遷移中でなければ sInvalid。
             */
            void RequestStageTransition(nsStage::StageID nextStageID);

            /**
             * @brief ステージ遷移を要求する。遷移中でなければ次ステージ ID を保存し、フェードアウトを開始する。
             * @param stageID 
             */
            void RefreshStageBackGround(nsStage::StageID stageID);

            /**
             * @brief ステージ遷移中の背景を更新する。遷移中でなければ何もしない。
             */
            inline bool IsStageTransitioning() const { return state_ != SceneTransitionState::None; }

            /**
             * @brief ステージ遷移中かどうかを返す。
             */
            inline nsApp::nsStage::StageID GetNextStageID() const { return nextStageID_; }

            /**
             * @brief 次ステージ ID を取得する。遷移中でなければ sInvalid。
             * @param mode カメラの状態。
             */
            void ChangeDimension(CameraMode mode);


        private:
            /**
             * @brief プレイヤーのインスタンスを取得する。
             */
            void PlayerCreateInstance();

            /**
             * @brief プレイヤーのインスタンスを生成する。
             */
            void UICreateInstance();

            /**
             * @brief UI のインスタンスを生成する。
             */
            void TimerCreateInstance();

            /**
             * @brief ステージ遷移中の更新処理。フェードアウト完了後に次ステージをロードする。
             */
            void NumberCreateInstance();

            /**
             * @brief ステージ遷移中の更新処理。フェードアウト完了後に次ステージをロードする。
             */
            void ScoreCreateInstance();

            /**
             * @brief スコア UI のインスタンスを生成する。
             */
            void HPbarCreateInstance();

            /**
             * @brief HPバー UI のインスタンスを生成する。
             */
            void UpdateTransition();


        private:
            InGameBuildHelper buildHelper_; //! 生成補助クラス。

            TimerUI* pTimerUI_ = nullptr; //! タイマー。
            NumberUI* pNumberUI_ = nullptr; //! スコア UI。
            ScoreUI* pScoreUI_ = nullptr;//! スコア。
            HPbarUI* pHpbarUI_ = nullptr;   //! HPBar UI。
            Fade* pFade_ = nullptr;//! フェード。
            Player* pPlayer_ = nullptr;     //! !  プレイヤー。
            CameraManager* pCameraManager_; //!  カメラマネージャ。
            nsStage::nsBackGround::IBackGround* pBackGrounds_ = nullptr; //! ステージ。
            nsK2EngineLow::GameSoundEngine* pSoundEngine_ = nullptr; //! 音源。


        private:
            SceneTransitionState state_ = SceneTransitionState::None; //! 遷移フロー。
            nsApp::nsStage::StageID nextStageID_ = nsApp::nsStage::StageID::sInvalid; //! ID。
            Stopwatch stageClearTimer_;//! ステージクリア時のタイマー。

            uint8_t currentStageNum_ = -1; //! 今のステージ数。

            bool m_isFadeInEnd = false;//! フェードしたか。
            bool m_hasAppliedStageBgm_ = false; //! フェードアウトしたか。

        private:
            /**
             * @brief ステージロードの段階を表す列挙型。
             */
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

// 後方互換（段階的に削除可）
namespace app
{
    namespace core
    {
        using Game = nsApp::nsCore::Game;
    }
} // namespace app
