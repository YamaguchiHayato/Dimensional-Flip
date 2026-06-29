#pragma once

/**
 * @file   StageManager.h
 * @brief  ステージの生成・切替・更新を管理するクラス。
 */

#include "ResultData.h"
#include "Src/Actor/Actor.h"
#include "Src/Actor/Stage/Stage.h"
#include "Src/Actor/Stage/StageID.h"
#include "Src/Actor/Stage/StageObjectSpawner.h"
#include "Src/Actor/Stage/StageSetup.h"

class LoadingScene;

namespace nsApp
{
    namespace nsStage
    {
        class StageManager : public Actor
        {
        private:
            Stage* pCurrentStage_ = nullptr;
            StageID stageCurrentID_ = StageID::sStage1;
            StageID stageRequestID_ = StageID::sInvalid;

            StageObjectSpawner spawner_; //! TSV から Star / Boss / CutIn などを生成
            StageSetup stageSetup_;      //! ボス戦カメラ・カットイン・Pause など

            static StageResultData stageResultData_;

        public:
            bool Start() override;
            void Update() override;
            void Render(RenderContext& rc) override;

            Stage* CreateStage(StageID id);
            void ChangeStageSync(StageID newStageID);

            inline void ChangeStage(StageID newStageID) { stageRequestID_ = newStageID; }

            static inline void SetStageResult(const StageResultData& data) { stageResultData_ = data; }

            Vector3 GetStageStartPos() const;
            inline StageID GetCurrentStageID() const { return stageCurrentID_; }
            static inline StageResultData& GetStageResultData() { return stageResultData_; }

            StageManager() = default;
            virtual ~StageManager();

        private:
            static StageManager* pStageManger_;
            static StageID nextInitStageID_;

        public:
            inline static void CreateInstance()
            {
                if (!pStageManger_)
                    pStageManger_ = NewGO<StageManager>(0, "stagemanager");
            }

            inline static void DeleteInstance()
            {
                DeleteGO(pStageManger_);
                pStageManger_ = nullptr;
            }

            inline static StageManager* GetInstance() { return pStageManger_; }
            inline static void SetNextInitStageID(StageID id) { nextInitStageID_ = id; }

            /** @brief ゲーム開始時に読み込むステージ ID を取得する。 */
            inline static StageID GetNextInitStageID() { return nextInitStageID_; }
        };
    } // namespace nsStage
} // namespace nsApp
