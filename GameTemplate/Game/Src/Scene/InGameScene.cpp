#include "stdafx.h"

#include "Src/Core/Game.h"
#include "Src/Core/SceneManager.h"
#include "Src/Core/StageManager.h"
#include "Src/Production/Fade.h"
#include "Src/Scene/InGameScene.h"

namespace nsApp
{
    namespace nsScene
    {
        InGameScene::~InGameScene()
        {
            /**
             * @brief インゲーム専用の描画状態を解除する
             */
            SceneManager::ResetRenderingStateForScene();

            /**
             * @brief Game GO を削除予約する
             */
            DeleteGO(pGame_);
        }

        bool InGameScene::Start()
        {
            /**
             * @brief インゲーム本体 GO を生成する
             */
            pGame_ = NewGO<Game>(0, "game");

            SceneManager::GetInstance()->GetFade()->StartFadeIn();
            return true;
        }

        void InGameScene::Update()
        {
            Fade* fade = SceneManager::GetInstance()->GetFade(); //! 共通フェード

            if (fade && fade->IsFadeOutEnd())
            {
                StageID current = nsStage::StageManager::GetInstance()->GetCurrentStageID(); //! 現在ステージ

                /**
                 * @brief 最終ステージクリア後はエンドロールへ遷移する
                 */
                if (current == StageID::sStageEX && pGame_->GetNextStageID() == StageID::sInvalid)
                    SceneManager::GetInstance()->ChangeScene(SceneID::sEndRoll);
            }
        }
    } // namespace nsScene
} // namespace nsApp
