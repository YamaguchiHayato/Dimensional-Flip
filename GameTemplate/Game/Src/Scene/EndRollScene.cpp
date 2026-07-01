#include "stdafx.h"

#include "EndRollScene.h"
#include "Src/Core/EndRollManager.h"
#include "Src/Core/SceneManager.h"
#include "Src/Production/Fade.h"



namespace nsApp
{
    namespace nsScene
    {
        EndRollScene::~EndRollScene()
        {
            if (pEndRollManager_)
            {
                DeleteGO(pEndRollManager_);
                pEndRollManager_ = nullptr;
            }

            // カメラの削除。
            if (pCamera_)
            {
                delete pCamera_;
                pCamera_ = nullptr;
            }
        }



        bool EndRollScene::Start()
        {
            // 時間の固定化を解除する。
            g_gameTime->DisableFixedFrameDeltaTime();

            pCamera_ = new nsK2EngineLow::Camera();

            // カメラをセット。
            pCamera_->SetPosition(Vector3(0.0f, 0.0f, -500.0f));
            pCamera_->SetTarget(Vector3::Zero);
            pCamera_->Update();
            g_camera3D = pCamera_;

            // エンドロールの管理クラスを生成。
            pEndRollManager_ = NewGO<nsProduction::EndRollManager>(0, "EndRollManager");

            return true;
        }

        void EndRollScene::Update()
        {
            if (pCamera_)
                pCamera_->Update();

            // EndRoll終了後、タイトルへ遷移。
            if (IsEnd())
                SceneManager::GetInstance()->ChangeScene(SceneID::sTitle);
        }

        bool EndRollScene::IsEnd() const
        {
            if (pEndRollManager_)
                return pEndRollManager_->IsEnd();

            return true;
        }
    } // namespace nsScene
} // namespace nsApp
