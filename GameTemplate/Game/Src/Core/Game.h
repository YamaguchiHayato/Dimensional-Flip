#pragma once
#include "stdint.h"
#include "GameSoundEngine.h"
#include "Src/Actor/Stage/IStage.h"
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
            bool Start();
            void Update();


        public:
            // ステージ遷移要求。
            void RequestStageTransition(StageID nextStageID);


        public:
            // ステージ遷移中かどうかを返す。
            inline bool IsStageTransitioning() const
            {
                return state_ != SceneTransitionState::None;
            }

            // 次のステージIDを取得する。
            inline StageID GetNextStageID() const
            {
                return nextStageID_;
            }

            // カメラのモードを切り替える共通関数。
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
            StageID nextStageID_ = StageID::sInvalid;
            Stopwatch stageClearTimer_;


        private:
            // 現在のステージ番号を追跡するための変数。
            uint8_t currentStageNum_ = -1;


        private:

            // フェードが終了しているかどうか
            bool m_isFadeInEnd = false;
            bool m_hasAppliedStageBgm_ = false;
        };
    }
}

