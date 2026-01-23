#include "stdafx.h"

#include "BattlePhaseManager.h"

#include "Src/Actor/Stage/Gimmick/FloatingPlatform.h"

#include "Src/Actor/Character/Player/Player.h"

namespace ScaffoldingPosList
{
    // 2D用パターン: ボスの正面(X軸)に向かって階段状に配置
    const std::vector<Vector3> PATTERN_2D =
    {
        Vector3(100.0f, 50.0f, 0.0f),
        Vector3(180.0f, 100.0f, 0.0f),
        Vector3(100.0f, 150.0f, 0.0f),
        Vector3(0.0f, 200.0f, 0.0f) // ボスの頭上付近
    };

    // 3D用パターン: ボスを中心に螺旋やジグザグに配置
    const std::vector<Vector3> PATTERN_3D =
    {
        Vector3(0.0f, 50.0f, 150.0f),
        Vector3(120.0f, 100.0f, 80.0f),
        Vector3(0.0f, 150.0f, 0.0f),
        Vector3(-100.0f, 200.0f, 0.0f) // ボスの頭上付近
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
            for (int i = 0; i < 4; i++)
            {
                CreatePlatform();
            }


            // フェーズを初期化
            currentPhase_ = app::enemyStatus::BossPhase::phase_One;

            
        }


        void BattlePhaseManager::Update()
        {
            // フェーズ3の自動切り替え処理などが必要になればここに記述
        }


        void BattlePhaseManager::AdvancePhase()
        {
            int next = static_cast<int>(currentPhase_) + 1;

            if (next < static_cast<int>(app::enemyStatus::BossPhase::phase_Num))
            {
                // フェーズを進める
                currentPhase_ = static_cast<app::enemyStatus::BossPhase>(next);

                // フェーズが進行すると環境を構築する。
                ApplyPhaseSettings();
            }
        }


        void BattlePhaseManager::ActivateScaffolding()
        {
            // フェーズに応じてパターンを選択
            const std::vector<Vector3>* currentPattern = nullptr;

            if (currentPhase_ == app::enemyStatus::BossPhase::phase_One)
                currentPattern = &ScaffoldingPosList::PATTERN_2D;

            else
                currentPattern = &ScaffoldingPosList::PATTERN_3D;

            // パターンが存在しないなら処理は中断。
            if (!currentPattern)
                return;

            // 足場を出現させる。
            SpawnPattern(*currentPattern);

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


        void BattlePhaseManager::SpawnPattern(const std::vector<Vector3>& pattern)
        {
            // パターンの座標数分だけ足場を有効化
            for (size_t i = 0; i < pattern.size(); ++i)
            {
                // プールしている足場の数を超えない範囲で使用
                if (i < platformsList_.size())
                    platformsList_[i]->Activate(pattern[i]);
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
