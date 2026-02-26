#pragma once
#include "Src/Actor/Stage/IStage.h"
#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Core/BossUIManager.h"


class Boss;
class Player;
class CameraManager;

namespace app
{
    namespace cutIn
    {
        class CutInView;
    }

    namespace gimmick
    {
        class FloatingPlatform;
    }
}


namespace app
{
    namespace nsStageEX
    {
        namespace nsBoss
        {
            const Vector3 InitPos = Vector3(10.0f, 5.0f, 0.0f);
        }

        namespace nsPlayer
        {
            const Vector3 InitPos =  Vector3(0.0f, 20.0f, 0.0f);
        }
    } 

    namespace stage
    {

        class StageEX : public IStage
        {
        public:
            StageEX() = default;
            virtual ~StageEX();


        public:
            bool Start() override;
            void Update() override;
            void Render(RenderContext& rc) override;


        public:
            inline Vector3 GetStageStartPos() const override
            {
                return nsStageEX::nsPlayer::InitPos;
            }

        private:
            inline void CreateBossInstance()
            {
                pBoss_ = NewGO<app::enemy::Boss>(0, "boss");
                pBoss_->SetPos(nsStageEX::nsBoss::InitPos);

                // Initialize BossUIManager
                app::nsUI::BossUIManager::GetInstance().Initialize();
            }


        private:
            app::enemy::Boss* pBoss_ = nullptr;
            CameraManager* pCameraManager_ = nullptr;
            Player* pPlayer_ = nullptr;
            app::cutIn::CutInView* pCutInView_ = nullptr;
            app::gimmick::FloatingPlatform* pFloatingPlatform_ = nullptr;


        private:
            bool isCutInPlayed_ = false;


            Quaternion rot_ = Quaternion::Identity;
        };

    }
}
