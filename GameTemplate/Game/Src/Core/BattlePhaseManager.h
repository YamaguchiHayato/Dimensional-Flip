#pragma once
#include "Src/Actor/Character/Enemy/Boss/BossType.h"
#include "Src/Collision/CollisionManager.h"
#include "Src/Core/CameraManager.h"
#include "Src/Core/Game.h"



namespace app
{
    namespace gimmick
    {
        class FloatingPlatform;
    }
} // namespace app

namespace app
{
    namespace core
    {
        class BattlePhaseManager
        {
        private:
            BattlePhaseManager() = default;
            virtual ~BattlePhaseManager() = default;

        private:
            // コピー&ムーブを禁止する。
            BattlePhaseManager(const BattlePhaseManager&) = delete;
            BattlePhaseManager& operator=(const BattlePhaseManager) = delete;

        private:
            // SingletonInstance。
            static BattlePhaseManager* pInstance_;

        public:
            // Instanceを取得。
            inline static BattlePhaseManager* GetInstance()
            {
                if (pInstance_ == nullptr)
                    pInstance_ = new BattlePhaseManager();

                return pInstance_;
            }

            // Instanceを削除。
            inline static void DeleteInstance()
            {
                if (pInstance_)
                {
                    delete pInstance_;
                    pInstance_ = nullptr;
                }
            }

        public:
            // フェーズを初期化。
            void Init();

            // 更新。
            void Update();

            // フェーズを進める。
            void AdvancePhase();

            // 足場を出現させる。
            void ActivateScaffolding();

            // 足場を非表示に。
            void DeactivateScaffolding();


            // ゲッター。
        public:
            // 現在のフェーズを取得。
            app::enemyStatus::BossPhase* GetCurrentPhase()
            {
                return &currentPhase_;
            };


            // ヘルパー。
        public:
            // 生成する足場の配列。
            void SpawnPattern(const std::vector<Vector3>& pattern, const Vector3& scale);

            // 保持している足場を全て削除。
            void ClearAllPlatforms();

        private:
            // 足場を生成する。
            void CreatePlatform();

            // フェーズに応じた必要なクラスを探索・取得。
            void ApplyPhaseSettings();

        private:
            app::enemyStatus::BossPhase currentPhase_ = app::enemyStatus::BossPhase::phase_One; // 現在のフェーズ。


        private:
            app::collision::CollisionManager* environmentCollison_ = nullptr;                   // 環境設定用コリジョンマネージャー。
            CameraManager* environmentCamera_ = nullptr;                                        // 環境設定用カメラマネージャー。
            std::vector<app::gimmick::FloatingPlatform*> platformsList_;                        // 浮遊プラットフォーム群。
        };

    } // namespace core
} // namespace app
