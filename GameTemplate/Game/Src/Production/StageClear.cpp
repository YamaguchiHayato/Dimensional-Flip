#include "stdafx.h"

#include "Fade.h"
#include "Src/Actor/Stage/Gimmick/StageGimmick/Star.h"
#include "Src/Core/SceneManager.h"
#include "Src/Core/StageManager.h"
#include "StageClear.h"

namespace nsApp
{
    namespace nsProduction
    {
        bool StageClear::Start()
        {
            stageClearSprite_.Init("Assets/sprite/stageClear.DDS", 324, 214);
            stageClearSprite_.SetScale(Vector3::One);
            return true;
        }


        void StageClear::Update()
        {
            timer_ += g_gameTime->GetFrameDeltaTime();
            stageClearSprite_.SetScale(Vector3::One);
            stageClearSprite_.Update();
        }


        void StageClear::Render(RenderContext& rc)
        {
            stageClearSprite_.Draw(rc);
        }


        StageID StageClear::DecideStage()
        {
            StageID currentStageID = nsStage::StageManager::GetInstance()->GetCurrentStageID();

            StageID nextStageID = currentStageID;

            switch (currentStageID)
            {
            case StageID::sTutorialStage:
                nextStageID = StageID::sStage1;
                break;

            case StageID::sStage1:
                nextStageID = StageID::sStageEX;
                break;

            case StageID::sStageEX:
                nextStageID = StageID::sInvalid;
                break;

            default:
                break;
            }

            return nextStageID;
        }
    } // namespace nsProduction
} // namespace nsApp
