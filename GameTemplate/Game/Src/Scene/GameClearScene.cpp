#include "stdafx.h"

#include "Src/Core/SceneManager.h"
#include "Src/Core/StageManager.h"
#include "Src/Production/Fade.h"
#include "Src/Scene/GameClearScene.h"

namespace nsApp
{
    namespace nsScene
    {
        GameClearScene::~GameClearScene()
        {
            /**
             * @brief リザルト演出 GO を削除する
             */
            if (pGameClear_)
            {
                DeleteGO(pGameClear_);
                pGameClear_ = nullptr;
            }

            /**
             * @brief 背景 SkyCube を削除する
             */
            if (pSkyCube_)
            {
                DeleteGO(pSkyCube_);
                pSkyCube_ = nullptr;
            }
        }

        bool GameClearScene::Start()
        {
            /**
             * @brief インゲーム描画状態を解除する
             */
            if (g_renderingEngine)
            {
                g_renderingEngine->EnableCompositeBackground(false);
                g_renderingEngine->SetStageBackGroundRenderer(nullptr);
            }

            /**
             * @brief 背景用 SkyCube を生成する
             */
            pSkyCube_ = NewGO<SkyCube>(0, "SkyCube");
            pSkyCube_->SetScale(Vector3::One * 1000.0f);
            pSkyCube_->SetType(EnSkyCubeType::enSkyCubeType_Wild);

            g_camera3D->SetPosition(Vector3::Zero);
            g_camera3D->SetTarget(Vector3::Zero);
            g_camera3D->Update();

            /**
             * @brief クリアデータを渡してリザルト UI を生成する
             */
            pGameClear_ = NewGO<GameClear>(0, "gameClear");
            const auto& data = nsStage::StageManager::GetStageResultData(); //! static なクリアデータ
            pGameClear_->SetUpResultData(data);

            SceneManager::GetInstance()->GetFade()->StartFadeIn();
            return true;
        }

        void GameClearScene::Update()
        {
            /**
             * @brief 演出終了後 A ボタンでタイトルへ戻る
             */
            if (pGameClear_ && pGameClear_->IsFinished())
            {
                if (g_pad[0]->IsTrigger(enButtonA))
                    SceneManager::GetInstance()->ChangeScene(nsScene::SceneID::sTitle);
            }
        }
    } // namespace nsScene
} // namespace nsApp
