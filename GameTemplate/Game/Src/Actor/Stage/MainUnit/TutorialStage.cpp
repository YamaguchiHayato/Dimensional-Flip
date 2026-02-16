#include "stdafx.h"
#include "Src/Actor/Stage/MainUnit/TutorialStage.h"

// カメラクラス。
#include "Src/Camera/Dimensiontrigger.h"
#include "Src/Actor/Character/Enemy/EnemyFactory.h"

#include "Src/Actor/Character/Player/Player.h"

// ギミッククラス。
#include "Src/Actor/Stage/Gimmick/StageGimmick/Star.h"
#include "Src/Actor/Stage/Gimmick/StageGimmick/Wall.h"

// 敵クラス。

namespace
{
    const Vector3 SCALE(1.0f, 1.0f, 1.0f);
    const Vector3 COLLISION_HEIGHT(0.0f, 50.0f, 0.0f);    // コリジョンの高さ
    const Vector3 COLLISION_SIZE(100.0f, 100.0f, 100.0f); // コリジョンの大きさ
} 

namespace app
{
    namespace stage
    {
        TutorialStage::~TutorialStage()
        {
            for (auto wall : wallList)
            {
                DeleteGO(wall);
            }

            // トリガーも同様に削除
            for (auto trigger : triggerList)
            {
                DeleteGO(trigger);
            }
            triggerList.clear();

            // 星も削除
            if (pStar_)
                DeleteGO(pStar_);

            wallList.clear();
        }


        bool TutorialStage::Start()
        {
            const std::string stagePath = InitStage("tutorialStage");
            stageRender_.Init(stagePath.c_str());

            stagePhysics_.CreateFromModel(stageRender_.GetModel(), stageRender_.GetModel().GetWorldMatrix());
            stageRender_.SetScale(SCALE);

            // 座標設定。
            stageRender_.SetPosition(stagePos_);
            initPos_ = stagePos_;

            // カメラを回せる範囲を指定。
            CreateCameraFlipRange();

            // 壁の生成。
            CreateWall();

            // 星の生成。
            CreateStar();

            // ボタンアクションUIの生成。
            CreateButtonActionUI();

            // チュートリアルメッセージUIの生成。
            CreateTutorialMessageUI();

            // NormalEnemyの生成。
    //        CreateNormalEnemy();

            pPlayer_ = FindGO<Player>("player");
            return true;
        }

        void TutorialStage::Update()
        {
            stageRender_.Update();

            // 当たり判定。
            // stagePhysics_.SetPosition(stagePos_);
        }

        void TutorialStage::Render(RenderContext& rc)
        {
            stageRender_.Draw(rc);
        }

        void TutorialStage::CreateCameraFlipRange()
        {
            std::vector<Vector3> TriggerList = {
                Vector3::Zero,
            };

            for (size_t i = 0; i < TriggerList.size(); i++)
            {
                cameraFlipRange_ = NewGO<DimensionTrigger>(0, "dimensiontrigger");
                cameraFlipRange_->SetTriggerPos(TriggerList[i]);
                triggerList.push_back(cameraFlipRange_);
            }
        }


        void TutorialStage::CreateWall()
        {
            std::vector<Vector3> TriggerList = {
                Vector3(193.0f, 3.0f, -9.3f),
            };

            for (size_t i = 0; i < TriggerList.size(); i++)
            {
                pWall_ = NewGO<app::stage::Wall>(0, "wall");
                pWall_->SetPos(TriggerList[i]);
                wallList.push_back(pWall_);
            }
        }


        void TutorialStage::CreateNormalEnemy()
        {
            auto enemy = app::enemy::EnemyFactory::CreateNormalSingle
            (
                Vector3(50.0f, 0.0f, 0.0f)
            );

            if (enemy)
                lEnemySpawnList_.push_back(enemy);
        }


        void TutorialStage::CreateTutorialMessageUI()
        {
            pTutorialMessageUI_ = NewGO<app::nsUI::TutorialMessageUI>(0, "TutorialMessageUI");
        }

    } 
}
