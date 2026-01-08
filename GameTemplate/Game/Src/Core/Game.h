#pragma once
#include "stdint.h"
#include "GameSoundEngine.h"
#include "Src/Actor/Stage/IStage.h"
#include "BackGroundLayer.h"
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


class CameraManager;
class Player;
class TrackingEnemy;
class Fade;
class LoadingScene;

// UI。
class NumberUI;
class ScoreUI;
class TimerUI;
class HPbarUI;

namespace app{
    namespace stage{
        class BackGroundLayer;
    }
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

            bool Start();
            void Update();
            void Render(RenderContext& rc);

            // ステージ遷移要求。
            void RequestStageTransition(StageID nextStageID);

        public:
            inline bool IsStageTransitioning() const
            {
                return state_ != SceneTransitionState::None;
            }
            inline StageID GetNextStageID() const
            {
                return nextStageID_;
            }
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
            // その他。
            void InitSkyCube(); // スカイキューブの初期化。

            // ステージ背景画像。
            void InitBackGround();
        private:
            // 遷移時の更新処理。
            void UpdateTransition();


        private:    
            TimerUI* pTimerUI_ = nullptr;
            NumberUI* pNumberUI_ = nullptr;
            ScoreUI* pScoreUI_ = nullptr;
            HPbarUI* pHpbarUI_ = nullptr;
            Fade* pFade_ = nullptr;
            LoadingScene* pLoadingScene_ = nullptr;
            SkyCube* pSkyCube_ = nullptr;
            Player* pPlayer_ = nullptr;
            CameraManager* pCameraManager_;

            nsK2EngineLow::GameSoundEngine* pSoundEngine_ = nullptr;

            // 背景レイヤー。
            app::stage::BackGroundLayer* pSkyLayer_ = nullptr;      // 空。
            app::stage::BackGroundLayer* pMountainLayer_ = nullptr; // 山。
            app::stage::BackGroundLayer* pCityLayer_ = nullptr;     // 街。
            app::stage::BackGroundLayer* pGroundLayer_ = nullptr;   // 地面。
            

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

