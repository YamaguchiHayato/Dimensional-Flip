#pragma once

#include "Src/Actor/Character/Enemy/Boss/BossType.h"
#include "Src/Collision/CollisionManager.h"
#include "Src/Core/CameraManager.h"
#include "Src/Core/Game.h"

/**
 * @file   BattlePhaseManager.h
 * @brief  ボス戦フェーズ中の浮遊足場（足場ギミック）を管理するシングルトン。
 */

namespace app
{
    namespace gimmick
    {
        class FloatingPlatform;
    }
} // namespace app

namespace nsApp
{
    namespace nsCore
    {
        /**
         * @class BattlePhaseManager
         * @brief ボス戦用の浮遊足場プールを生成・表示・非表示にする。
         * @note  カメラモード（2D/3D）に応じて足場パターンを切り替える。
         */
        class BattlePhaseManager
        {
        public:
            /**
             * @brief シングルトンインスタンスを取得する。
             * @return BattlePhaseManager のインスタンス。未生成なら new する。
             */
            inline static BattlePhaseManager* GetInstance()
            {
                if (pInstance_ == nullptr)
                    pInstance_ = new BattlePhaseManager();
                return pInstance_;
            }

            /**
             * @brief シングルトンインスタンスを破棄する。
             */
            inline static void DeleteInstance()
            {
                if (pInstance_)
                {
                    delete pInstance_;
                    pInstance_ = nullptr;
                }
            }

            /**
             * @brief 足場プールを初期化する。既存足場は全削除してから再生成する。
             */
            void Init();

            /**
             * @brief 毎フレームの更新処理（予約）。
             */
            void Update();

            /**
             * @brief 現在のカメラモードに応じた足場パターンを出現させる。
             */
            void ActivateScaffolding();

            /**
             * @brief 保持している足場をすべて非表示にする。
             */
            void DeactivateScaffolding();

            /**
             * @brief 視点切り替えの間隔（秒）を設定する。
             * @param interval 切り替え間隔（秒）。
             */
            inline void SetChangeInterval(float interval) { changeInterval_ = interval; }

            /**
             * @brief 視点切り替えタイマーの初期値を設定する。
             * @param timer タイマー初期値（秒）。
             */
            inline void SetDimensionTimer(float timer) { dimensionTimer_ = timer; }

            /**
             * @brief 指定パターンの座標・スケールで足場を有効化する。
             * @param pattern 各足場のワールド座標リスト。
             * @param scale   各足場に適用するスケール。
             */
            void SpawnPattern(const std::vector<Vector3>& pattern, const Vector3& scale);

            /**
             * @brief 保持している足場 GO をすべて削除する。
             */
            void ClearAllPlatforms();

        private:
            /* コンストラクタとデストラクタ。*/
            BattlePhaseManager() = default;
            virtual ~BattlePhaseManager() = default;

            BattlePhaseManager(const BattlePhaseManager&) = delete;
            BattlePhaseManager& operator=(const BattlePhaseManager&) = delete;

            /**
             * @brief 足場 GO を1つプール用に生成する。
             */
            void CreatePlatform();

        private:
            static BattlePhaseManager* pInstance_; //!< シングルトンインスタンス。

            CollisionManager* environmentCollison_ = nullptr; //!< 環境コリジョン（予約）。
            CameraManager* environmentCamera_ = nullptr;                      //!< 環境カメラ（予約）。
            std::vector<app::gimmick::FloatingPlatform*> platformsList_;      //!< 浮遊足場プール。

            Player* pPlayer_ = nullptr; //!< プレイヤー参照（予約）。

            float dimensionTimer_ = 0.0f;  //!< 視点切り替えタイマー。
            float changeInterval_ = 15.0f; //!< 視点切り替え間隔（秒）。

            CameraMode currentCamMode_; //!< 現在のカメラモード（予約）。
            CameraMode nextCamMode_;    //!< 次のカメラモード（予約）。
        };
    } // namespace nsCore
} // namespace nsApp

using BattlePhaseManager = nsApp::nsCore::BattlePhaseManager;

namespace app
{
    namespace core
    {
        using BattlePhaseManager = nsApp::nsCore::BattlePhaseManager;
    }
} // namespace app
