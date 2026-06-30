#pragma once

/**
 * @file   ManualUI.h
 * @brief  タイトル画面の操作説明オーバーレイ。
 */

namespace nsApp
{
    namespace nsUI
    {
        /**
         * @class ManualUI
         * @brief コントローラー画像と各ボタン説明を表示する。
         */
        class ManualUI : public IGameObject
        {
        public:
            ManualUI() = default;
            virtual ~ManualUI() = default;

            /** @brief スプライト・フォントを初期化する。 */
            void Init();

            /** @brief 表示中スプライトの更新処理。 */
            void Update();

            /**
             * @brief UI を描画する。
             * @param rc レンダリングコンテキスト。
             */
            void Render(RenderContext& rc);

            /**
             * @brief UI を表示する。
             */
            inline void Open()  { isActive_ = true; }

            /**
             * @brief UI を非表示にする。
             */
            inline void Close() { isActive_ = false; }

            /**
             * @brief UI の表示状態を取得する。
             * @return 表示中なら true。
             */
            inline bool IsActive() const { return isActive_; }

        private:
            /**
             * @brief 各スプライト・フォントを初期化する。
             */
            void InitManualText();
            void InitGuideLine();
            void InitBackButton();
            void InitTitleText();
            void InitMoveText();
            void InitCameraActionText();
            void InitJumpText();
            void InitBackText();

        private:
            SpriteRender manualSprite_;         //!< コントローラー画像
            SpriteRender moveLine_;             //!< 移動ガイドライン
            SpriteRender jumpLine_;             //!< ジャンプガイドライン
            SpriteRender cameraActionLine_;     //!< カメラ操作ガイドライン
            SpriteRender buttonActionUI_;       //!< B ボタン画像
            FontRender backTextFont_;           //!< Back 表記
            FontRender titleFont_;              //!< タイトル表記
            FontRender jumpTextFont_;           //!< ジャンプ説明
            FontRender cameraActionTextFont_;   //!< カメラ操作説明
            FontRender moveTextFont_;           //!< 移動説明
            bool isActive_ = false;             //!< 表示フラグ
        };
    } // namespace nsUI
} // namespace nsApp

using ManualUI = nsApp::nsUI::ManualUI;
