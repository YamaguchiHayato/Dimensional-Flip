#include "stdafx.h"

#include "Src/Actor/Character/Player/Player.h"
#include "Src/Core/StageManager.h"
#include "Src/Parameter/Stage/StageMasterTable.h"
#include "Src/UI/NumberUI.h"
#include "Src/UI/Tutorial/TutorialMessageUI.h"

namespace nsApp
{
    namespace nsStage
    {
        StageManager* StageManager::pStageManger_ = nullptr;
        StageID StageManager::nextInitStageID_ = StageID::sTutorialStage;
        StageResultData StageManager::stageResultData_;

        namespace
        {
            void SetupTutorialUI(StageID stageId)
            {
                /* Tutorialが存在する確認。*/
                auto* pExisting = FindGO<TutorialMessageUI>("TutorialMessageUI");

                if (stageId == StageID::sTutorialStage)
                {
                    /* Tutorialが存在しない場合は生成する。*/
                    if (!pExisting)
                        NewGO<TutorialMessageUI>(0, "TutorialMessageUI");
                }
                else if (pExisting)
                    DeleteGO(pExisting);
            }
        } // namespace


        StageManager::~StageManager()
        {
            /* StageManagerのインスタンスが自分自身である場合は、静的メンバをnullptrにする。*/
            if (pStageManger_ == this)
                pStageManger_ = nullptr;

            /* TutorialUIを削除する。*/
            SetupTutorialUI(StageID::sInvalid);

            /* 現在のステージが存在する場合は、ステージを削除する。*/
            spawner_.Clear();
            stageSetup_.OnLeave(stageCurrentID_);

            /* 現在のステージを削除する。*/
            DeleteGO(pCurrentStage_);
            pCurrentStage_ = nullptr;
        }


        bool StageManager::Start()
        {
            /* StageManagerのインスタンスを静的メンバに設定する。*/
            pCurrentStage_ = CreateStage(nextInitStageID_);
            if (pCurrentStage_ == nullptr)
                return false;

            /* 現在のステージIDを次の初期化ステージIDに設定する。*/
            stageCurrentID_ = nextInitStageID_;

            /* ステージをスポーンする。*/
            spawner_.Spawn(stageCurrentID_);
            SetupTutorialUI(stageCurrentID_);
            stageSetup_.OnEnter(stageCurrentID_);

            return true;
        }


        void StageManager::Update()
        {
            /* ステージ変更要求がある場合は、同期的にステージを変更する。*/
            if (stageRequestID_ != StageID::sInvalid)
            {
                ChangeStageSync(stageRequestID_);
                stageRequestID_ = StageID::sInvalid;
            }

            /* ステージセットアップの更新を行う。*/
            stageSetup_.Update();

            /* 現在のステージが存在する場合は、ステージの更新を行う。*/
            if (pCurrentStage_ != nullptr)
                pCurrentStage_->Update();
        }


        void StageManager::Render(RenderContext& rc)
        {
            /* 現在のステージが存在する場合は、ステージの描画を行う。*/
            if (pCurrentStage_ != nullptr)
                pCurrentStage_->Render(rc);

            /* ステージセットアップの描画を行う。*/
            stageSetup_.Render(rc);
        }


        void StageManager::ChangeStageSync(StageID newStageID)
        {
            /* 現在のステージIDと新しいステージIDが同じで、現在のステージが存在する場合は、何もしない。*/
            if (stageCurrentID_ == newStageID && pCurrentStage_ != nullptr)
                return;

            /* ステージスポナーをクリアする。*/
            spawner_.Clear();
            stageSetup_.OnLeave(stageCurrentID_);

            /* 現在のステージが存在する場合は、ステージを削除する。*/
            if (pCurrentStage_ != nullptr)
            {
                DeleteGO(pCurrentStage_);
                pCurrentStage_ = nullptr;
            }

            /* 新しいステージを作成する。*/
            Stage* pNextStage = CreateStage(newStageID);
            if (pNextStage == nullptr)
                return;

            /* 現在のステージを新しいステージに設定する。*/
            pCurrentStage_ = pNextStage;
            stageCurrentID_ = newStageID;

            /* ステージスポナーを使用して新しいステージをスポーンする。*/
            spawner_.Spawn(newStageID);
            SetupTutorialUI(newStageID);
            stageSetup_.OnEnter(newStageID);

            /* プレイヤーの位置をステージの開始位置に設定する。*/
            if (auto* pPlayer = FindGO<Player>("player"))
            {
                const Vector3 startPos = GetStageStartPos();
                pPlayer->SetPlayerPos(startPos);
                pPlayer->SetRespwanPos(startPos);
            }

            /* NumberUIのタイマーをリセットする。*/
            if (auto* pTimer = FindGO<NumberUI>("numberui"))
                pTimer->ResetTimer();
        }


        Stage* StageManager::CreateStage(StageID id)
        {
            /* ステージマスターテーブルからステージ情報を取得する。*/
            const auto& master = nsSystem::StageMasterTable::Get(id);
            if (master.stageID == StageID::sInvalid)
                return nullptr;

            /* 新しいステージを生成する。*/
            auto* stage = NewGO<Stage>(0, "stage");
            if (stage == nullptr)
                return nullptr;

            /* ステージの初期化を行う。*/
            stage->SetStageID(id);
            return stage;
        }


        Vector3 StageManager::GetStageStartPos() const
        {
            /* 現在のステージが存在する場合は、ステージの開始位置を取得する。*/
            if (pCurrentStage_ != nullptr)
                return pCurrentStage_->GetPlayerStartPos();

            /* 現在のステージIDが有効な場合は、ステージマスターテーブルから開始位置を取得する。*/
            const auto& master = nsSystem::StageMasterTable::Get(stageCurrentID_);
            if (master.stageID != StageID::sInvalid)
                return master.playerStartPosition;

            return Vector3(0.0f, 20.0f, 0.0f);
        }
    } // namespace nsStage
} // namespace nsApp
