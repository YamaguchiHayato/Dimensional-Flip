#pragma once
#include "Src/Actor/Character/Player/Player.h"

class CameraManager;

namespace
{
    static constexpr int BG_COUNT = 10;
    static constexpr auto BG_WIDTH = 150.0f;
    static constexpr int SCROLL_NUM = 4;
}

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
            // 座標のオーバーライド設定。
            inline void SetOverrideTrackingPos(const Vector3& pos)
            {
                isOverrideTracking_ = true;
                overridePos_ = pos;
            }

            // オーバーライド設定をクリア。
            inline void ClearOverride()
            {
                isOverrideTracking_ = false;
            }


        public:
            // 座標をセットする。
            inline void SetPosition(const Vector3& position)
            {
                position_ = position;
            }


        private:
            // ステージ背景モデル(空)の設定項目。
            void SettingSky();


        private:
            Player* pPlayer_ = nullptr;
            CameraManager* pCameraManager_ = nullptr;


        private:
            ModelRender skyModel_[BG_COUNT];        // 空


            Vector3 position_;
            Vector3 overridePos_ = Vector3::Zero;
            Vector3 lastTrackingPos_ = Vector3::Zero;

            Quaternion rot_ = Quaternion::Identity;

            bool isOverrideTracking_ = false;
        };
    } 
} 
