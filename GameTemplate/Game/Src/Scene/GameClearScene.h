#pragma once

#include "Src/Production/GameClear.h"
#include "Src/Scene/Scene.h"

/**
 * @file   GameClearScene.h
 * @brief  ステージクリア後のリザルトシーン。
 */

namespace nsApp
{
    namespace nsScene
    {
        /**
         * @class GameClearScene
         * @brief  GameClear 演出と StageResultData 表示を行う。
         * @note   SetAmbientByIBLTexture は使わない（ModelRender 一斉 ReInit 回避）。
         */
        class GameClearScene : public IScene
        {
        public:
            /* コンストラクタとデストラクタ。*/
            GameClearScene() = default;
            virtual ~GameClearScene();


        public:
            /**
             * @brief シーン開始時の初期化処理。
             * @return true: 初期化成功、false: 初期化失敗
             */
            bool Start() override;

            /**
             * @brief シーン更新処理。
             */
            void Update() override;


        private:
            GameClear* pGameClear_ = nullptr; //!< リザルト UI 演出
            SkyCube* pSkyCube_ = nullptr;     //!< 背景用（任意。2D のみなら削除可）

            bool isTransitionRequested_ = false; //! < ステージ遷移リクエスト済みかどうかのフラグ >
        };
    }
}

using GameClearScene = nsApp::nsScene::GameClearScene;
