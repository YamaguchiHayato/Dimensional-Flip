#pragma once
#include "Src/Actor/Stage/IStage.h"
#include "Src/Actor/Character/Enemy/Boss/Boss.h"


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
            const Vector3 InitPos = Vector3(0.0f, 100.0f, 200.0f);
        }

        namespace nsPlayer
        {
            const Vector3 InitPos =  Vector3(0.0f, 100.0f, -300.0f);
        }
    } 

    namespace stage
    {

        class StageEX : public IStage
        {
        public:
            StageEX() = default;
            virtual ~StageEX() = default;

            bool Start() override;
            void Update() override;
            void Render(RenderContext& rc) override;

            inline Vector3 GetStageStartPos() const override
            {
                return nsStageEX::nsPlayer::InitPos;
            }

        private:
            inline void BossInstance()
            {
                pBoss_ = NewGO<app::enemy::Boss>(0, "boss");
                pBoss_->SetPos(nsStageEX::nsBoss::InitPos);   
            }


        private:
            app::enemy::Boss* pBoss_ = nullptr;
            CameraManager* pCameraManager_ = nullptr;
            Player* pPlayer_ = nullptr;
            app::cutIn::CutInView* pCutInView_ = nullptr;
            app::gimmick::FloatingPlatform* pFloatingPlatform_ = nullptr;
        private:

            bool isCutInPlayed_ = false;
        };

    }
}
