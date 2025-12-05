#pragma once
#include "Src/Actor/Stage/IStage.h"
#include "Src/Actor/Character/Enemy/Boss.h"


class Boss;
class Player;
class CameraManager;

namespace app
{
    namespace cutIn
    {
        class CutInView;
    }
}


namespace app
{
    namespace nsStageEX
    {
        namespace nsBoss
        {
            const Vector3 InitPos = Vector3(0.0f, 250.0f, 0.0f);
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

            inline Vector3 GetStageStartPos() const override { return Vector3(0.0f, 200.0f, 0.0f); }

        private:
            inline void BossInstance()
            {
                pBoss_ = NewGO<Boss>(0, "boss");
                pBoss_->SetPos(nsStageEX::nsBoss::InitPos);   
            }


        private:
            Boss* pBoss_ = nullptr;
            CameraManager* pCameraManager_ = nullptr;
            Player* pPlayer_ = nullptr;
            app::cutIn::CutInView* pCutInView_ = nullptr;

        private:

            bool isCutInPlayed_ = false;
        };

    }
}
