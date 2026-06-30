#pragma once

#include <cstdint>

/**
 * @file   Scene.h
 * @brief  シーン基底インターフェースとシーン ID 列挙型。
 */

namespace nsApp
{
    namespace nsScene
    {
        /**
         * @enum SceneID
         * @brief ゲーム内シーンを識別する列挙型。
         */
        enum class SceneID : uint8_t
        {
            sFade = 0,   //!< フェード専用（予約）
            sTitle,      //!< タイトル
            sWorldSelect,//!< ステージ選択
            sInGame,     //!< インゲーム
            sResult,     //!< クリアリザルト
            sGameOver,   //!< ゲームオーバー
            sEndRoll,    //!< エンドロール
            sInvalid = 0xff
        };

        /**
         * @class IScene
         * @brief 全シーンが実装するライフサイクルインターフェース。
         * @note  GO の Update/Render は GameObjectManager が呼ぶ。シーンは遷移と非 GO 処理を担当。
         */
        class IScene
        {
        public:
            /* コンストラクタとデストラクタ。*/
            IScene() = default;
            virtual ~IScene() = default;

            /**
             * @brief シーン入場時の初期化。必要な GO を NewGO する。
             * @return 成功時 true。
             */
            virtual bool Start() = 0;

            /**
             * @brief 毎フレームのシーン更新（遷移判定・入力など）。
             */
            virtual void Update() = 0;
        };
    } // namespace nsScene
} // namespace nsApp

using SceneID = nsApp::nsScene::SceneID;
using IScene  = nsApp::nsScene::IScene;
