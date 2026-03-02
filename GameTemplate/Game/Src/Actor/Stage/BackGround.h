#pragma once
#include "Src/Actor/Character/Player/Player.h"

class CameraManager;

namespace
{
    static constexpr int BG_COUNT = 10;
    static constexpr auto BG_WIDTH = 150.0f; // 1枚あたりの横幅
} 


enum class EnBackGroundType : uint8_t
{
    type_NormalStage,
    type_BossBattle
};


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


        // セッター。
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

            // 座標をセットする。
            inline void SetPosition(const Vector3& position)
            {
                position_ = position;
            }

        public: 
            // 背景タイプを設定する（ここでモデルの読み込みを走らせる）
            void SetBackGroundType(EnBackGroundType type);


        private:
            // 実際にモデルをロード・配置する内部関数
            void SettingBackGround();


        private:
            Player* pPlayer_ = nullptr;
            CameraManager* pCameraManager_ = nullptr;


        private:
            ModelRender backGroundModels_[BG_COUNT];

            EnBackGroundType backGroundType_ = EnBackGroundType::type_NormalStage;

            Vector3 position_;
            Vector3 bossBackGroundPosition_ = Vector3::Zero;
            Vector3 overridePos_ = Vector3::Zero;
            Vector3 lastTrackingPos_ = Vector3::Zero;
            Vector3 backGroundPosition_ = Vector3::Zero;

            Quaternion rot_ = Quaternion::Identity;

            bool isOverrideTracking_ = false;
            int typeIndex_ = 0;
            int dataSize_ = 0;
        };
    } // namespace stage
} // namespace app
