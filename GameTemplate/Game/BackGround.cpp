#include "stdafx.h"

#include "BackGround.h"

#include "Src/Core/CameraManager.h"
#include "Src/Core/StageManager.h"

namespace
{
    const float BG_Z_OFFSET = 50.0f;
    const float HEIGHT_SKY = 8.0f;
    const float HEIGHT_GROUND = -5.0f;
    const Vector3 SCALE_2D = Vector3(0.15f, 0.15f, 0.15f);

    const float BASE_HEIGHT = 25.0f;
    const float BASE_SCALE = 0.15f;

    // 山の設定
    struct MountainSetting
    {
        static constexpr float WIDTH = 800.0f;           // 画像幅
        static constexpr float SCROLL_START_X = 200.0f;  // スクロール開始位置
        static constexpr float SCROLL_SPEED = 0.5f;      // 速度係数
        static constexpr float HEIGHT_MOUNTAIN = -50.0f; // 高さ

        // カリング距離
        static constexpr float CULLING_RANGE = 1200.0f;
    };
}


namespace app
{
    namespace stage
    {
        bool BackGround::Start()
        {
            if (isInitialized_)
                return true;

            InitModel();

            pPlayer_ = FindGO<Player>("player");
            pCameraManager_ = FindGO<CameraManager>("cameramanager");

            isInitialized_ = true;
            return true;
        }


        void BackGround::Update()
        {
            if (!pPlayer_ || !pCameraManager_)
                return;

            if (pCameraManager_->GetCurrentCameraMode() != CameraMode::mode2D)
                return;

            // プレイヤー座標取得
            Vector3 playerPos = pPlayer_->GetPlayerPos();

            // 共通の回転を計算
            rot_.SetRotationDegX(-90.0f);

            // ステージによって背景の挙動を変更する。
            StageID currentStage = StageManager::GetInstance()->GetCurrentStageID();


            if (currentStage == StageID::sStageEX)
            {
                Vector3 camPos = g_camera3D->GetPosition();
                float currentCamHeight = g_camera3D->GetHeight();

                // 位置: カメラに合わせる
                skyModel_.SetPosition({camPos.x, camPos.y, BG_Z_OFFSET});

                float newScale = (currentCamHeight / BASE_HEIGHT) * BASE_SCALE * 2.0f;

                skyModel_.SetScale({newScale, newScale, newScale});
                skyModel_.SetRotation(rot_);
                skyModel_.Update();
            }

            else
            {
                Vector3 playerPos = pPlayer_->GetPlayerPos();
                SettingSky(playerPos);
            }

        //    SettingGround(playerPos);   // 単体として処理
        //    SettingMountain(playerPos); // 配列として処理
        }

        void BackGround::Render(RenderContext& rc)
        {
            if (!pCameraManager_ || pCameraManager_->GetCurrentCameraMode() != CameraMode::mode2D)
                return;

            if (!pPlayer_)
                return;

            skyModel_.Draw(rc);
            groundModel_.Draw(rc);

            // 山の描画
            if (pPlayer_->GetPlayerPos().x > MountainSetting::SCROLL_START_X)
            {
                Vector3 playerPos = pPlayer_->GetPlayerPos();

                for (int i = 0; i < SCROLL_NUM; i++)
                {
                    Vector3 pos = mountainModel_[i].GetPosition();
                    float dist = fabsf(pos.x - playerPos.x);

                    if (dist < MountainSetting::CULLING_RANGE)
                    {
                        mountainModel_[i].Draw(rc);
                    }
                }
            }
        }


        void BackGround::InitModel()
        {
            skyModel_.Init("Assets/stage/BackGround/Sky.tkm");

            // 地面 (単体)
            groundModel_.Init("Assets/stage/BackGround/ground.tkm");

            // 山 (配列)
            for (int i = 0; i < SCROLL_NUM; i++)
            {
                mountainModel_[i].Init("Assets/stage/BackGround/Sky.tkm");
            }
        }


        void BackGround::SettingSky(const Vector3& playerPos)
        {
            skyModel_.SetRotation(rot_);
            skyModel_.SetScale(SCALE_2D);
            skyModel_.SetPosition({playerPos.x, HEIGHT_SKY, BG_Z_OFFSET});
            skyModel_.Update();
        }


        void BackGround::SettingGround(const Vector3& playerPos)
        {
            float groundZ = BG_Z_OFFSET - 10.0f;

            groundModel_.SetRotation(rot_);
            groundModel_.SetScale(SCALE_2D);

            groundModel_.SetPosition({playerPos.x, HEIGHT_GROUND, groundZ});

            groundModel_.Update();
        }


        void BackGround::SettingMountain(const Vector3& playerPos)
        {
            if (playerPos.x <= MountainSetting::SCROLL_START_X)
                return;

            float dist = playerPos.x - MountainSetting::SCROLL_START_X;
            float scrollVal = dist * (1.0f - MountainSetting::SCROLL_SPEED);
            float offset = std::fmod(scrollVal, MountainSetting::WIDTH);

            for (int i = 0; i < SCROLL_NUM; i++)
            {
                float drawX = (playerPos.x - offset) + (i * MountainSetting::WIDTH) -
                              (MountainSetting::WIDTH * (SCROLL_NUM * 0.5f - 0.5f));

                Vector3 pos;
                pos.x = drawX;
                pos.y = MountainSetting::HEIGHT_MOUNTAIN;
                pos.z = BG_Z_OFFSET - 5.0f;

                mountainModel_[i].SetRotation(rot_);
                mountainModel_[i].SetScale(SCALE_2D);
                mountainModel_[i].SetPosition(pos);
                mountainModel_[i].Update();
            }
        }
    } 
} 
