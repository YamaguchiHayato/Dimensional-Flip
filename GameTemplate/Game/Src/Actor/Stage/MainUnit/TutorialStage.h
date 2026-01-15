#pragma once
#include "Src/Actor/Stage/IStage.h"
#include "Src/Actor/Character/Enemy/IEnemy.h"


#include "Src/Actor/Stage/Gimmick/Wall.h"
#include "Src/Actor/Stage/Gimmick/Star.h"
#include "Src/Actor/Character/Enemy/EnemyFactory.h"


class Star;
class Player;
class DimensionTrigger;


namespace app{
    namespace stage{
        class Wall;
    }

    namespace ui{
        class ButtonActionUI;
    }
}
namespace app
{
    namespace stage
    {
        class TutorialStage : public IStage
        {
        public:
            TutorialStage() = default;
            virtual ~TutorialStage();

        public:
            bool Start() override;
            void Update() override;
            void Render(RenderContext& rc) override;

        public:
            inline const std::string InitStage(const std::string& stagename) override
            {
                return IStage::InitStage(stagename);
            };


        public:
            inline Vector3 GetStageStartPos() const override
            {
                return Vector3(0.0f, 20.0f, 0.0f);
            }

        private:
            // 星オブジェクトを生成。
            inline void CreateStar()
            {
                pStar_ = NewGO<Star>(0, "star");
                pStar_->SetStarPosition(Vector3(262.2f, 25.0f, 0.0f));
            }

            // カメラの回転範囲。
            void CreateCameraFlipRange();

            // 壁の生成。
            void CreateWall();

            // UI描画の生成。
            inline void CreateButtonActionUI()
            {
                pCreateButtonUI_ = NewGO<app::ui::ButtonActionUI>(0, "actionUI");
            }

            // NormalEnemyの生成。
            inline void CreateNormalEnemy();
        private:
            Player* pPlayer_ = nullptr;
            Star* pStar_ = nullptr;
            DimensionTrigger* cameraFlipRange_ = nullptr;
            Wall* pWall_ = nullptr;
            app::ui::ButtonActionUI* pCreateButtonUI_ = nullptr;

            // 敵生成リスト。
            std::vector<app::enemy::IEnemy*> lEnemySpawnList_;

        private:
            std::vector<DimensionTrigger*> triggerList;
            std::vector<app::stage::Wall*> wallList;

        private:
            Vector3 pos_ = Vector3::One;
            Vector3 scale_ = Vector3::One;
            Vector3 basePos_ = Vector3::Zero;
            Quaternion rot_ = Quaternion::Identity;
            Quaternion rot_90_Y = Quaternion::Identity;
        };

    }
}
