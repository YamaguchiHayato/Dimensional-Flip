#pragma once

#include <vector>

#include "Src/Scene/Scene.h"
#include "StageIcon.h"

/**
 * @file   WorldSelectScene.h
 * @brief  ステージ選択シーン。
 */

namespace nsApp
{
    namespace nsProduction { class Fade; }
    namespace nsUI         { class WorldSelectUI; class StageIcon; }

    namespace nsScene
    {
        /**
         * @class WorldSelectScene
         * @brief SkyCube 背景と StageIcon 回転でステージを選ぶ。
         * @note  SetAmbientByIBLTexture は使わず SetAmbient のみ（IBL 漏れ防止）。
         */
        class WorldSelectScene : public IScene
        {
        public:
            /* コンストラクタとデストラクタ。*/
            WorldSelectScene() = default;
            virtual ~WorldSelectScene();


        public:
            /**
             * @brief シーン入場時の初期化。必要な GO を NewGO する。
             * @return 成功時 true。
             */
            bool Start() override;

            /**
             * @brief 毎フレームのシーン更新（遷移判定・入力など）。
             */
            void Update() override;


        private:
            /**
             * @brief SkyCube 背景を生成する。
             */
            void CreateSkyCube();


        private:
            nsProduction::Fade* pFade_ = nullptr; //! フェード用 GO。
            nsUI::WorldSelectUI* pSelectUI_ = nullptr; //! ステージ選択 UI 用 GO。
            SkyCube* pSkyCube_ = nullptr;              //! < SkyCube 背景用 GO。
            std::vector<nsUI::StageIcon*> icons_;      //! < ステージアイコン GO の配列。
            int currentIndex_ = 0;                     //!< 現在選択中のステージアイコンのインデックス。
            float currentAngle_ = 0.0f;                //! < 現在の回転角度。
            float targetAngle_ = 0.0f;                 //! < 目標の回転角度。
            bool isDecided_ = false;                   //! < ステージが決定されたかどうか。
            bool isButtonB_ = false;                   //! < B ボタンが押されたかどうか。
            bool isRight_ = false;                     //! < 右方向入力中かどうか。
            bool isLeft_ = false;                      //! < 左方向入力中かどうか。
        };
    } // namespace nsScene
} // namespace nsApp

using WorldSelectScene = nsApp::nsScene::WorldSelectScene;
