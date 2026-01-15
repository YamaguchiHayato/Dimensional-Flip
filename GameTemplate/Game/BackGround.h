#pragma once
#include "Src/Actor/Character/Player/Player.h"

class CameraManager;

namespace app
{
    namespace stage
    {
        class BackGround : public IGameObject
        {
        public:
            BackGround() = default;
            virtual ~BackGround() = default;

        public:
            bool Start() override;
            void Update() override;
            void Render(RenderContext& rc) override;

        public:
            inline void SetPosition(const Vector3& position) { position_ = position; }

        private:
            void InitModel();

            void SettingSky(const Vector3& playerPos);
            void SettingMountain(const Vector3& playerPos);
            void SettingGround(const Vector3& playerPos);

        private:
            Player* pPlayer_ = nullptr;
            CameraManager* pCameraManager_ = nullptr;

        private:
            static constexpr int SCROLL_NUM = 4;

            ModelRender skyModel_;                  // 空
            ModelRender groundModel_;               // 地面
            ModelRender mountainModel_[SCROLL_NUM]; // 山

            Vector3 position_;
            Quaternion rot_ = Quaternion::Identity;

            // 二重初期化防止フラグ
            bool isInitialized_ = false;
        };
    } // namespace stage
} // namespace app
