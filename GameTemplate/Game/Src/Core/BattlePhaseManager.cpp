#include "stdafx.h"

#include "BattlePhaseManager.h"
#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Actor/Character/Enemy/Boss/BossAttackState.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Stage/Gimmick/BossGimmick/FloatingPlatform.h"
#include "Src/Core/Game.h"

namespace
{
    const Vector3 SCALE_2D = Vector3(0.1f, 0.1f, 0.1f);
    const Vector3 SCALE_3D = Vector3(0.35f, 0.1f, 0.35f);

    /**
     * @brief 2D 視点用の足場配置パターン（階段状）
     */
    const std::vector<Vector3> PATTERN_2D = {Vector3(-25.0f, 8.0f, 0.0f), Vector3(-10.0f, 16.0f, 0.0f),
                                             Vector3(10.0f, 24.0f, 0.0f), Vector3(20.0f, 32.0f, 0.0f)};

    /**
     * @brief 3D 視点用の足場配置パターン（ジグザグ）
     */
    const std::vector<Vector3> PATTERN_3D = {
        Vector3(0.0f, 5.0f, 20.0f),    Vector3(-30.0f, 8.0f, 12.0f),  Vector3(30.0f, 11.0f, 4.0f),
        Vector3(-30.0f, 14.0f, -4.0f), Vector3(30.0f, 17.0f, -12.0f), Vector3(0.0f, 20.0f, -20.0f),
    };
} // namespace

namespace nsApp
{
    namespace nsCore
    {
        BattlePhaseManager* BattlePhaseManager::pInstance_ = nullptr;

        void BattlePhaseManager::Init()
        {
            /**
             * @brief 既存足場を削除してからプールを再構築する
             */
            ClearAllPlatforms();

            for (int i = 0; i < 6; i++)
                CreatePlatform();
        }

        void BattlePhaseManager::ActivateScaffolding()
        {
            const std::vector<Vector3>* currentPattern = nullptr;
            Vector3 currentScale = SCALE_2D;

            /**
             * @brief プレイヤーのカメラモードで 2D/3D パターンを選択
             */
            auto* pPlayer = FindGO<Player>("player");
            if (!pPlayer || !pPlayer->GetCameraManager())
                return;

            CameraMode mode = pPlayer->GetCameraManager()->GetCurrentCameraMode();

            if (mode == CameraMode::mode2D)
            {
                currentPattern = &PATTERN_2D;
                currentScale = SCALE_2D;
            }
            else
            {
                currentPattern = &PATTERN_3D;
                currentScale = SCALE_3D;
            }

            if (currentPattern)
                SpawnPattern(*currentPattern, currentScale);
        }

        void BattlePhaseManager::DeactivateScaffolding()
        {
            /**
             * @brief 全足場を非表示にする（GO は保持）
             */
            for (auto* platform : platformsList_)
            {
                if (platform)
                    platform->Deactivate();
            }
        }

        void BattlePhaseManager::SpawnPattern(const std::vector<Vector3>& pattern, const Vector3& scale)
        {
            /**
             * @brief パターン座標数ぶん、プール内の足場を有効化する
             */
            for (size_t i = 0; i < pattern.size(); ++i)
            {
                if (i < platformsList_.size())
                    platformsList_[i]->Activate(pattern[i], scale);
            }
        }

        void BattlePhaseManager::ClearAllPlatforms()
        {
            /**
             * @brief プール内の足場 GO をすべて DeleteGO する
             */
            for (auto* platform : platformsList_)
            {
                if (platform)
                    DeleteGO(platform);
            }
            platformsList_.clear();
        }

        void BattlePhaseManager::CreatePlatform()
        {
            /**
             * @brief 非表示状態で足場を1つ生成しプールに追加する
             */
            auto* platform = NewGO<app::gimmick::FloatingPlatform>(0);
            platform->Deactivate();
            platformsList_.push_back(platform);
        }
    } // namespace nsCore
} // namespace nsApp
