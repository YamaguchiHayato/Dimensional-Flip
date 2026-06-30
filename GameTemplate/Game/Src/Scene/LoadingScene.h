#pragma once

/**
 * @file   LoadingScene.h
 * @brief  シーン遷移中のロード画面 GO。
 */

namespace nsApp
{
    namespace nsScene
    {
        class LoadingScene : public IGameObject
        {
        public:
            /* コンストラクタとデストラクタ。*/
            LoadingScene() = default;
            virtual ~LoadingScene() = default;

        public:
            /**
             * @brief シーン入場時の初期化。必要な GO を NewGO する。
             * @return 成功時 true。
             */
            bool Start();

            /**
             * @brief 毎フレームのシーン更新（点滅処理）。
             */
            void Update();

            /**
             * @brief ロード画面を描画する。
             * @param rc レンダリングコンテキスト。
             */
            void Render(RenderContext& rc);


        private:
            /**
             * @brief ロード画面のフォントを初期化する。
             */
            void CreateLoadingFont();


        private:
            FontRender loadingFont_; //!< Now Loading 表示
            float timer_ = 0.0f;     //!< 点滅用タイマー
        };
    }
}

using LoadingScene = nsApp::nsScene::LoadingScene;
