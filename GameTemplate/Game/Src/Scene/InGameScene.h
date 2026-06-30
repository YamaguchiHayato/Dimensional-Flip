#pragma once

#include "Src/Scene/Scene.h"

/**
 * @file   InGameScene.h
 * @brief  インゲームシーン。Game GO の生成と破棄を担当。
 */

namespace app
{
    namespace core
    {
        class Game;
    }
}

namespace nsApp
{
    namespace nsScene
    {
        /**
         * @class InGameScene
         * @brief  Game オブジェクトのライフサイクルを管理する IScene 実装。
         */
        class InGameScene : public IScene
        {
        public:
            /* コンストラクタとデストラクタ。*/
            InGameScene() = default;
            virtual ~InGameScene();


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
            app::core::Game* pGame_ = nullptr; //!< インゲーム本体
        };
    }
}

using InGameScene = nsApp::nsScene::InGameScene;
