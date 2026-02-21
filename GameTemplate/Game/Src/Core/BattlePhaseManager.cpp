#include "stdafx.h"

#include "BattlePhaseManager.h"
#include "Src/Core/BossUIManager.h"

#include "Src/Actor/Character/Enemy/Boss/BossAttackState.h"

#include "Src/Actor/Stage/Gimmick/BossGimmick/FloatingPlatform.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Character/Enemy/Boss/Boss.h"
#include "Src/Core/Game.h"

namespace ScaffoldingStatus
{
    const Vector3 SCALE_2D = Vector3(0.1f, 0.1f, 0.1f);
    const Vector3 SCALE_3D = Vector3(0.35f, 0.1f, 0.35f);

    // 2D用パターン: ボスの正面(X軸)に向かって階段状に配置
    const std::vector<Vector3> PATTERN_2D = {
        Vector3(-25.0f, 8.0f, 0.0f),   // 1段目 (左端)
        Vector3(-10.0f, 16.0f, 0.0f),  // 2段目 (中央)
        Vector3(10.0f, 24.0f, 0.0f),   // 3段目 (右端)
        Vector3(20.0f, 32.0f, 0.0f)    // 4段目 (中央)
    };


    // 3D用パターン: ボスを中心に螺旋やジグザグに配置
    const std::vector<Vector3> PATTERN_3D = {
        Vector3(0.0f, 5.0f, 20.0f),     // 1: 手前中央 (スタート)
        Vector3(-30.0f, 8.0f, 12.0f),   // 2: 左へ大きく移動
        Vector3(30.0f, 11.0f, 4.0f),    // 3: 右へ大きく移動
        Vector3(-30.0f, 14.0f, -4.0f),  // 4: 左へ大きく移動
        Vector3(30.0f, 17.0f, -12.0f),  // 5: 右へ大きく移動
        Vector3(0.0f, 20.0f, -20.0f),   // 6: 奥中央 (ボスの目の前)
    };
}

namespace app
{
    namespace core
    {
        // 静的メンバ変数の実体定義
        BattlePhaseManager* BattlePhaseManager::pInstance_ = nullptr;


        void BattlePhaseManager::Init()
        {
            // 既存の足場があれば削除してリセット
            ClearAllPlatforms();


            // 足場をプール生成
            for (int i = 0; i < 6; i++)
            {
                CreatePlatform();
            }

        }


        void BattlePhaseManager::ActivateScaffolding()
        {
            // フェーズに応じてパターンを選択
            const std::vector<Vector3>* currentPattern = nullptr;
            Vector3 currentScale = ScaffoldingStatus::SCALE_2D;

            // 現在のカメラモードを取得。
            auto* pPlayer = FindGO<Player>("player");
            if (!pPlayer || !pPlayer->GetCameraManager())
                return;

            // 現在のカメラモードを取得。
            CameraMode mode = pPlayer->GetCameraManager()->GetCurrentCameraMode();


            // Phase1なら2D用、Phase2以降なら3D用を選択
            if (mode == CameraMode::mode2D)
            {
                // 足場生成リスト。
                currentPattern = &ScaffoldingStatus::PATTERN_2D;

                // 2D用の大きさ。
                currentScale = ScaffoldingStatus::SCALE_2D;
            }

            else
            {
                // 足場生成リスト。
                currentPattern = &ScaffoldingStatus::PATTERN_3D;

                // 3D用の大きさ。
                currentScale = ScaffoldingStatus::SCALE_3D;
            }


            // パターンが存在しないなら処理は中断。
            if (currentPattern)
                // 足場を出現させる。
                SpawnPattern(*currentPattern, currentScale);
        }


        void BattlePhaseManager::DeactivateScaffolding()
        {
            // 全ての足場を非表示にする
            for (auto* platform : platformsList_)
            {
                if (platform)
                    platform->Deactivate();
            }
        }


        void BattlePhaseManager::SpawnPattern(const std::vector<Vector3>& pattern, const Vector3& scale)
        {
            // パターンの座標数分だけ足場を有効化
            for (size_t i = 0; i < pattern.size(); ++i)
            {
                if (i < platformsList_.size())
                    platformsList_[i]->Activate(pattern[i], scale);
            }
        }


        void BattlePhaseManager::ClearAllPlatforms()
        {
            for (auto* platform : platformsList_)
            {
                if (platform)
                    DeleteGO(platform);
            }
            platformsList_.clear();
        }


        void BattlePhaseManager::CreatePlatform()
        {
            auto* platform = NewGO<app::gimmick::FloatingPlatform>(0);
            platform->Deactivate(); // 初期状態は非表示
            platformsList_.push_back(platform);
        }
    } 
} 
