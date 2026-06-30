#pragma once

#include "Src/Scene/Scene.h"

/**
 * @file   SceneManager.h
 * @brief  シーン遷移・ロード画面・フェードを統括するマネージャ。
 */

namespace nsApp
{
    namespace nsProduction { class Fade; }
    namespace nsScene     { class LoadingScene; }

    namespace nsCore
    {
        /**
         * @enum SceneTransitionState
         * @brief Game 内ステージ遷移用（SceneManager とは別系統）。
         */
        enum class SceneTransitionState : uint8_t
        {
            None,
            FadeOut,
            Load,
            Load_Render,
            Load_Wait,
            Load_WaitFinish,
            FadeIn,
            FadeIn_Wait,
        };

        /**
         * @class SceneManager
         * @brief 全シーンの生成・破棄・遷移タイミングを管理するシングルトン。
         */
        class SceneManager : public nsScene::IScene
        {
        private:
            nsScene::IScene* pCurrentScene_ = nullptr; //! < 現在のシーンインスタンス。nullptr なら遷移中。
            nsScene::SceneID currentID_ = nsScene::SceneID::sTitle; //!< 現在のシーン ID。
            nsScene::SceneID requestID_ = nsScene::SceneID::sInvalid; //!< 遷移リクエストされたシーン ID。sInvalid なら遷移なし。
            nsScene::SceneID targetID_ = nsScene::SceneID::sInvalid; //!< 遷移先のシーン ID。sInvalid なら遷移なし。

            static SceneManager* pSceneManger_; //!< シングルトンインスタンス。
            nsProduction::Fade* pFade_ = nullptr; //!< フェードイン・アウトを担当するシーン。nullptr なら未生成。
            nsScene::LoadingScene* pLoadingScene_ = nullptr; //! < ロード画面を担当するシーン。nullptr なら未生成。

            bool isLoadingSceneActive_ = false; //! < ロード画面がアクティブかどうか。
            bool isSceneControler_ = false;//! < シーン遷移中にシーンの Update を呼ぶかどうか。ロード画面がアクティブなら false。
            bool isAutoLoadingEnabled_ = true; //! < フェードアウト中の自動ロード表示を有効にするかどうか。
            float minLoadingTime_ = 0.0f;      //! < ロード画面の最小表示時間。0.0f なら制限なし。


        public:
            /**
             * @brief シーンマネージャの初期化。フェードとロード画面を生成する。
             * @return 成功時 true。
             */
            bool Start() override;

            /**
             * @brief シーンマネージャの更新。シーン遷移リクエストがあればフェードアウトを開始する。
             */
            void Update() override;

            /**
             * @brief 指定 ID のシーンインスタンスを生成する。
             * @param id シーン ID。
             * @return 生成したシーン。失敗時 nullptr。
             */
            nsScene::IScene* CreateScene(nsScene::SceneID id);

            /**
             * @brief ロード画面を表示する。
             */
            void ShowLoading();

            /**
             * @brief ロード画面を非表示にする。
             */
            void HideLoading();

            /**
             * @brief フェードアウト中の自動ロード表示を有効/無効にする。
             * @param enable true で有効。
             */
            void SetEnableAutoLoading(bool enable) { isAutoLoadingEnabled_ = enable; }

            /**
             * @brief ロード画面の最小表示時間を設定する。
             * @param newID 遷移先シーン ID。
             */
            inline void ChangeSceneInternal(nsScene::SceneID newID) { requestID_ = newID; }

            /**
             * @brief シーン遷移をリクエストする。
             * @param newID 遷移先シーン ID。
             */
            inline void ChangeScene(nsScene::SceneID newID) { requestID_ = newID; }

            /**
             * @brief シーン退場時に描画エンジンのインゲーム専用状態を解除する。
             * @details Composite 背景とステージ背景コールバックを解除。
             */
            static void ResetRenderingStateForScene()
            {
                if (!g_renderingEngine)
                    return;
                g_renderingEngine->EnableCompositeBackground(false);
                g_renderingEngine->SetStageBackGroundRenderer(nullptr);
            }

            /* コンストラクタとデストラクタ。*/
            SceneManager() = default;
            virtual ~SceneManager();


        public:
            /**
             * @brief シングルトンインスタンスを生成する。すでに生成済みなら何もしない。
             */
            inline static void CreateInstance()
            {
                if (!pSceneManger_)
                    pSceneManger_ = new SceneManager();
            }

            /**
             * @brief フェードシーンを取得する。
             * @return フェードシーン。nullptr なら未生成。
             */
            inline nsProduction::Fade* GetFade() { return pFade_; }

            /**
             * @brief ロード画面シーンを取得する。
             * @return ロード画面シーン。nullptr なら未生成。
             */
            nsScene::SceneID GetTargetSceneID() const { return targetID_; }

            /**
             * @brief シングルトンインスタンスを取得する。
             * @return シングルトンインスタンス。未生成なら nullptr。
             */
            inline static SceneManager* GetInstance() { return pSceneManger_; }

            /**
             * @brief シングルトンインスタンスを破棄する。すでに破棄済みなら何もしない。
             */
            inline static void DeleteInstance()
            {
                delete pSceneManger_;
                pSceneManger_ = nullptr;
            }
        };
    } // namespace nsCore
} // namespace nsApp

using SceneTransitionState = nsApp::nsCore::SceneTransitionState;
using SceneManager         = nsApp::nsCore::SceneManager;
