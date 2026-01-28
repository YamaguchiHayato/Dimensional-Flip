#include "stdafx.h"

#include "BattlePhaseManager.h"
#include "Src/Core/BossUIManager.h"

#include "Src/Actor/Stage/Gimmick/FloatingPlatform.h"

#include "Src/Actor/Character/Player/Player.h"

namespace ScaffoldingStatus
{
    const Vector3 SCALE_2D = Vector3(0.1f, 0.1f, 0.1f);
    const Vector3 SCALE_3D = Vector3(0.35f, 0.1f, 0.35f);

    // 2D用パターン: ボスの正面(X軸)に向かって階段状に配置
    const std::vector<Vector3> PATTERN_2D = {
        Vector3(-25.0f, 8.0f, 0.0f),  // 1段目 (左端)
        Vector3(-10.0f, 16.0f, 0.0f), // 2段目 (中央)
        Vector3(0.0f, 24.0f, 0.0f),   // 3段目 (右端)
        Vector3(10.0f, 32.0f, 0.0f)   // 4段目 (中央)
    };


    // 3D用パターン: ボスを中心に螺旋やジグザグに配置
    const std::vector<Vector3> PATTERN_3D = {
        Vector3(0.0f, 5.0f, 20.0f),     // 1: 正面手前
        Vector3(14.0f, 10.0f, 14.0f),   // 2: 右手前
        Vector3(20.0f, 15.0f, 0.0f),    // 3: 右真横
        Vector3(14.0f, 20.0f, -14.0f),  // 4: 右奥
        Vector3(0.0f, 25.0f, -20.0f),   // 5: 真裏
        Vector3(-14.0f, 30.0f, -14.0f), // 6: 左奥
        Vector3(-20.0f, 35.0f, 0.0f),   // 7: 左真横
        Vector3(-14.0f, 40.0f, 14.0f)   // 8: 左手前
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
            for (int i = 0; i < 8; i++)
            {
                CreatePlatform();
            }


            // フェーズを初期化
            currentPhase_ = app::enemyStatus::BossPhase::phase_One;
        }


        void BattlePhaseManager::Update()
        {
            // フェーズ3の自動切り替え処理などが必要になればここに記述
            // @ TODO フェーズ3の次はStaffRollSceneに移動予定。 
        }


        void BattlePhaseManager::AdvancePhase()
        {
            int next = static_cast<int>(currentPhase_) + 1;

            if (next < static_cast<int>(app::enemyStatus::BossPhase::phase_Num))
            {
                // フェーズを進める
                currentPhase_ = static_cast<app::enemyStatus::BossPhase>(next);

                // UIにフェーズ変更を通知。
                app::nsUI::BossUIManager::GetInstance().OnChangePhase((uint8_t) currentPhase_);

                // フェーズが進行すると環境を構築する。
                ApplyPhaseSettings();
            }
        }


        void BattlePhaseManager::ActivateScaffolding()
        {
            // フェーズに応じてパターンを選択
            const std::vector<Vector3>* currentPattern = nullptr;
            Vector3 currentScale = ScaffoldingStatus::SCALE_2D;

            // Phase1なら2D用、Phase2以降なら3D用を選択
            if (currentPhase_ == app::enemyStatus::BossPhase::phase_One)
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


        void BattlePhaseManager::ApplyPhaseSettings()
        {
            auto* pGame = FindGO<app::core::Game>("game");
            if (!pGame)
                return;


            if (currentPhase_ == app::enemyStatus::BossPhase::phase_One)
                // フェーズ1は強制的に2Dへ
                pGame->ChangeDimension(CameraMode::mode2D);

            else
                // それ以外は強制的に3Dへ
                pGame->ChangeDimension(CameraMode::mode3D);
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
