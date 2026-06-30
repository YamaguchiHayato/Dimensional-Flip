#include "stdafx.h"

#include "Src/Actor/Character/Player/Player.h"
#include "Src/Core/StageManager.h"
#include "Src/Parameter/Stage/StageMasterTable.h"
#include "Src/UI/NumberUI.h"

namespace nsApp
{
    namespace nsStage
    {
        StageManager* StageManager::pStageManger_ = nullptr;
        StageID StageManager::nextInitStageID_ = StageID::sTutorialStage;
        StageResultData StageManager::stageResultData_;


        StageManager::~StageManager()
        {
            if (pStageManger_ == this)
                pStageManger_ = nullptr;

            spawner_.Clear();
            stageSetup_.OnLeave(stageCurrentID_);

            DeleteGO(pCurrentStage_);
            pCurrentStage_ = nullptr;
        }


        bool StageManager::Start()
        {
            pCurrentStage_ = CreateStage(nextInitStageID_);
            if (pCurrentStage_ == nullptr)
                return false;

            stageCurrentID_ = nextInitStageID_;

            // TSV からオブジェクト生成（Boss / CutIn 含む）
            spawner_.Spawn(stageCurrentID_);
            // ボス戦ルール適用
            stageSetup_.OnEnter(stageCurrentID_);

            return true;
        }


        void StageManager::Update()
        {
            if (stageRequestID_ != StageID::sInvalid)
            {
                ChangeStageSync(stageRequestID_);
                stageRequestID_ = StageID::sInvalid;
            }

            // カットイン終了・BossUI 更新
            stageSetup_.Update();

            if (pCurrentStage_ != nullptr)
                pCurrentStage_->Update();
        }


        void StageManager::Render(RenderContext& rc)
        {
            if (pCurrentStage_ != nullptr)
                pCurrentStage_->Render(rc);

            // Boss HP バーなど
            stageSetup_.Render(rc);
        }


        void StageManager::ChangeStageSync(StageID newStageID)
        {
            if (stageCurrentID_ == newStageID && pCurrentStage_ != nullptr)
                return;

            // 古いステージのオブジェクトとルールを片付け
            spawner_.Clear();
            stageSetup_.OnLeave(stageCurrentID_);

            if (pCurrentStage_ != nullptr)
            {
                DeleteGO(pCurrentStage_);
                pCurrentStage_ = nullptr;
            }

            Stage* pNextStage = CreateStage(newStageID);
            if (pNextStage == nullptr)
                return;

            pCurrentStage_ = pNextStage;
            stageCurrentID_ = newStageID;

            // 新ステージのオブジェクト生成
            spawner_.Spawn(newStageID);
            // 新ステージのルール適用（ボス戦ならカメラ・Pause）
            stageSetup_.OnEnter(newStageID);

            if (auto* pPlayer = FindGO<Player>("player"))
            {
                const Vector3 startPos = GetStageStartPos();
                pPlayer->SetPlayerPos(startPos);
                pPlayer->SetRespwanPos(startPos);
            }

            if (auto* pTimer = FindGO<NumberUI>("numberui"))
                pTimer->ResetTimer();
        }


        Stage* StageManager::CreateStage(StageID id)
        {
            const auto& master = nsSystem::StageMasterTable::Get(id);
            if (master.stageID == StageID::sInvalid)
                return nullptr;

            auto* stage = NewGO<Stage>(0, "stage");
            if (stage == nullptr)
                return nullptr;

            stage->SetStageID(id);
            return stage;
        }


        Vector3 StageManager::GetStageStartPos() const
        {
            if (pCurrentStage_ != nullptr)
                return pCurrentStage_->GetPlayerStartPos();

            const auto& master = nsSystem::StageMasterTable::Get(stageCurrentID_);
            if (master.stageID != StageID::sInvalid)
                return master.playerStartPosition;

            return Vector3(0.0f, 20.0f, 0.0f);
        }
    } // namespace nsStage
} // namespace nsApp
